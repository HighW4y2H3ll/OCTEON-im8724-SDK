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
 * * @file serdes_core.c
 ** @brief Functions for SerDes devices.
 **/

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_serdes.h"
#include "avago_serdes_pmd.h"
#include "avago_sbus.h"
#include "avago_spico.h"
#include "avago_avsp.h"
#include "avago_logging.h"
#include "avago_str_conv.h"

#define AVAGO_LOG_PRINT5 if (ah->debug >= 5) avago_log_printf
#define AVAGO_LOG_PRINT6 if (ah->debug >= 6) avago_log_printf


/** @defgroup SerDes Generic SerDes API */
/** @{ */

/**
 * @brief	Gets the SerDes LSB revision.
 *
 * @param	ah	Avago handle
 * @param	addr	Device address number
 *
 * @details	 Reads the revision of SerDes LSB using an AVAGO_LSB_DIRECT read./
 *
 * @return	Revision of the LSB (> 0), or -1 on error.
 *
 * @see avago_firmware_get_rev(), avago_firmware_get_build_id().
 */
int avago_serdes_get_lsb_rev(struct avago_hdl *ah, uint32_t addr)
{
	int rc = -1;
	if (!avago_check_process(ah, addr, __func__, __LINE__, true, 3,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F, AVAGO_PROCESS_A))
		return rc;

	if (avago_check_ip_type(ah, addr, __func__, __LINE__, true, 3,
				AVAGO_SERDES, AVAGO_M4, AVAGO_P1)) {
		if (avago_check_process(ah, addr, __func__, __LINE__, false, 2,
					AVAGO_PROCESS_B, AVAGO_PROCESS_F)) {
			rc = avago_serdes_mem_rd(ah, addr, AVAGO_LSB_DIRECT,
						 0x26);
			rc = (rc >> 12) & 0xf;
		} else if (avago_check_process(ah, addr, __func__, __LINE__,
					       false, 1, AVAGO_PROCESS_A)) {
			rc = (avago_sbus_rd(ah, addr, 0x64) >> 28) & 0xf;

		}
	}

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, ret = %d.\n", avago_addr_to_str(addr), rc);

	return rc;
}

static int get_mem_slice_sel(struct avago_hdl *ah, uint32_t addr)
{
	int slice = (addr >> 16) & 0xf;
	if ((slice < 8 && slice != 2) ||
	    avago_get_ip_type(ah, addr) != AVAGO_P1)
		return 0;
	if (slice == 2)
		slice = 4;
	return slice & 7;
}

/** @brief   Reads a memory location from a SerDes device. */
/** */
/** @return  Value read.  On error, ah->return_code is set to negative value. */
/** @see     avago_serdes_mem_wr(), avago_serdes_mem_rmw(). */
int avago_serdes_mem_rd(struct avago_hdl *ah,	/**< [in] Pointer to struct avago_hdl structure. */
			uint32_t addr,	    /**< [in] Device address number. */
			enum avago_serdes_mem_type type,
					/**< [in] The memory access type. */
			uint32_t mem_addr)
{
/**< [in] Memory address to access */
	int ret = 0;
	int sdrev = avago_get_sdrev(ah, addr);
	if (sdrev != AVAGO_SDREV_D6 && sdrev != AVAGO_SDREV_HVD6) {
		switch (type) {
		case AVAGO_DMEM:
		case AVAGO_ESB:
			type = AVAGO_LSB;
			break;
		case AVAGO_ESB_DIRECT:
			type = AVAGO_LSB_DIRECT;
			break;
		default:
			break;
		}
	}

	switch (type) {
	case AVAGO_LSB:
		if (avago_get_spico_running_flag(ah, addr) ||
		    avago_set_spico_running_flag(ah, addr,
						 avago_spico_running(ah,
								     addr))) {
			ret = avago_spico_int(ah, addr,
					      (1 << 14) | (mem_addr & 0x03ff),
					      0x00);
			break;
		}

	case AVAGO_LSB_DIRECT:
		if (sdrev == AVAGO_SDREV_P1) {
			int spico_run_state = avago_spico_halt(ah, addr);

			int sel = get_mem_slice_sel(ah, addr);
			if (mem_addr >= 128 && mem_addr < 384)
				avago_sbus_wr(ah, addr, 0x2d, sel);
			else if (mem_addr >= 384 && mem_addr < 448)
				mem_addr += sel * 64 + 1024 - 384;

			avago_sbus_wr(ah, addr, 0x01,
				      (1 << 30) | (mem_addr & 0x0fff));
			ret = (avago_sbus_rd(ah, addr, 0x01) >> 12) & 0xFFFF;

			avago_spico_resume(ah, addr, spico_run_state);
		} else if (sdrev != AVAGO_SDREV_D6 && sdrev != AVAGO_SDREV_HVD6) {
			avago_sbus_wr(ah, addr, 0x01,
				      (1 << 30) | (mem_addr & 0x03ff));
			ret = (avago_sbus_rd(ah, addr, 0x01) >> 12) & 0xFFFF;
		} else {
			avago_sbus_wr(ah, addr, 0x02,
				      ((mem_addr & 0x1ff) << 16));
			ret = avago_sbus_rd(ah, addr, 0x40) & 0xFFFF;
		}
		break;

	case AVAGO_ESB:
		if (avago_get_spico_running_flag(ah, addr) ||
		    avago_set_spico_running_flag(ah, addr,
						 avago_spico_running(ah,
								     addr))) {
			avago_spico_int_check(ah, __func__, __LINE__, addr, 0x18, 0x4000 | (mem_addr & 0x3fff));
			ret = avago_spico_int(ah, addr, 0x1a, 0x00);
			break;
		}

	case AVAGO_ESB_DIRECT:
		{
			int lsb_esb_addr;
			int spico_run_state = avago_spico_halt(ah, addr);

			int reset_status = avago_sbus_rd(ah, addr, 0x07);
			if (reset_status & 0x01)
				avago_sbus_wr(ah, addr, 0x07, reset_status & ~1);

			lsb_esb_addr =
			    avago_serdes_mem_rd(ah, addr, AVAGO_LSB_DIRECT,
						0x030);

			if (lsb_esb_addr & 0x8000)
				avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
						    0x030,
						    lsb_esb_addr & 0x7fff);

			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x30, mem_addr);
			ret = avago_serdes_mem_rd(ah, addr, AVAGO_LSB_DIRECT,
						  0x32);

			if (lsb_esb_addr & 0x8000)
				avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
						    0x30,
						    lsb_esb_addr & 0x7fff);
			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x30, lsb_esb_addr);

			if (reset_status & 0x01)
				avago_sbus_wr(ah, addr, 0x07, reset_status);
			avago_spico_resume(ah, addr, spico_run_state);
			break;
		}

	case AVAGO_DMEM:
	case AVAGO_DMEM_PREHALTED:
		if (sdrev == AVAGO_SDREV_HVD6) {
			avago_sbus_wr(ah, addr, 0x01,
				      (1 << 29) | (1 << 30) | (mem_addr &
							       0x03ff));
			ret = (avago_sbus_rd(ah, addr, 0x01) >> 12) & 0xFFFF;
			break;
		} else {
			int spico_run_state = 0, initial_value_0x01;
			if (type == AVAGO_DMEM)
				spico_run_state = avago_spico_halt(ah, addr);
			initial_value_0x01 =
			    avago_sbus_rmw(ah, addr, 0x01,
					   0x40000000 | (mem_addr & 0xfff),
					   0x40000fff);
			ret = (avago_sbus_rd(ah, addr, 0x01) >> 12) & 0xffff;
			avago_sbus_wr(ah, addr, 0x01, initial_value_0x01);
			if (type == AVAGO_DMEM)
				avago_spico_resume(ah, addr, spico_run_state);
			break;
		}

	case AVAGO_IMEM:
	case AVAGO_IMEM_PREHALTED:
		{
			int spico_run_state = 0;
			if (type == AVAGO_IMEM)
				spico_run_state = avago_spico_halt(ah, addr);
			avago_sbus_wr(ah, addr, 0x00,
				      (mem_addr & 0xffff) | 1 << 30);
			ret = (avago_sbus_rd(ah, addr, 0x00) >> 16) & 0x3ff;
			avago_sbus_wr(ah, addr, 0x00, 0x00);
			if (type == AVAGO_IMEM)
				avago_spico_resume(ah, addr, spico_run_state);
			break;
		}

	default:
		printf("ERROR: (%s:%d): SBus 0x%x, Invalid DMA type (%d).\n",
		       __func__, __LINE__, addr, type);
		return 0;
	}

	AVAGO_LOG_PRINT6(ah, AVAGO_DEBUG6, __func__, __LINE__,
			 "SBus %s, Read  %s 0x%04x -> 0x%x\n",
			 avago_addr_to_str(addr), avago_mem_type_to_str(type),
			 mem_addr, ret);
	return ret;
}

/** @brief   Writes data to a memory location. */
/** */
/** @return  On error, decrements ah->return_code. */
/** @see     avago_serdes_mem_rd(), avago_serdes_mem_rmw(). */
void avago_serdes_mem_wr(struct avago_hdl *ah,	/**< [in] Pointer to struct avago_hdl structure. */
			 uint32_t addr,	    /**< [in] Device address number. */
			 enum avago_serdes_mem_type type,
					/**< [in] The memory access type. */
			 uint32_t mem_addr, /**< [in] Memory address to access */
			 uint32_t data)
{
/**< [in] Data to write into memory location addr. */
	int sdrev = avago_get_sdrev(ah, addr);
	AVAGO_LOG_PRINT6(ah, AVAGO_DEBUG6, __func__, __LINE__,
			 "SBus %s, Write %s 0x%04x <- 0x%x\n",
			 avago_addr_to_str(addr),
			 avago_mem_type_to_str(type), mem_addr, data);
	if (sdrev != AVAGO_SDREV_D6 && sdrev != AVAGO_SDREV_HVD6) {
		switch (type) {
		case AVAGO_DMEM:
		case AVAGO_ESB:
			type = AVAGO_LSB;
			break;
		case AVAGO_ESB_DIRECT:
			type = AVAGO_LSB_DIRECT;
			break;
		default:;
		}
	}

	switch (type) {
	case AVAGO_LSB:
		if (avago_get_spico_running_flag(ah, addr) ||
		    avago_set_spico_running_flag(ah, addr,
						 avago_spico_running(ah,
								     addr))) {
			avago_spico_int(ah, addr,
					(2 << 14) | (mem_addr & 0x03ff), data);
			return;
		}

	case AVAGO_LSB_DIRECT:
		if (sdrev == AVAGO_SDREV_P1) {
			int spico_run_state = avago_spico_halt(ah, addr);
			int sel = get_mem_slice_sel(ah, addr);
			int reset_status = avago_sbus_rd(ah, addr, 0x07);
			if ((reset_status & 0x01) || !(reset_status & 0x10))
				avago_sbus_wr(ah, addr, 0x07,
					      (reset_status & ~1) | 0x10);

			if (mem_addr >= 128 && mem_addr < 384)
				avago_sbus_wr(ah, addr, 0x2d, sel);
			else if (mem_addr >= 384 && mem_addr < 448)
				mem_addr += sel * 64 + 1024 - 384;
			avago_sbus_wr(ah, addr, 0x01,
				      (1 << 31) | ((data & 0xffff) << 12) |
				      (mem_addr & 0x0fff));

			if ((reset_status & 0x01) || !(reset_status & 0x10))
				avago_sbus_wr(ah, addr, 0x07, reset_status);
			avago_spico_resume(ah, addr, spico_run_state);
		} else if (sdrev != AVAGO_SDREV_D6 && sdrev != AVAGO_SDREV_HVD6) {
			avago_sbus_wr(ah, addr, 0x01,
				      (1 << 31) | ((data & 0xffff) << 12) |
				      (mem_addr & 0x03ff));
		} else {
			if (avago_get_lsb_rev(ah, addr) >= 3) {
				avago_sbus_wr(ah, addr, 0x02,
					      (0 << 31) |
					      ((mem_addr & 0x1ff) << 16) |
					      (data & 0xffff));
			}
			avago_sbus_wr(ah, addr, 0x02,
				      (1 << 31) | ((mem_addr & 0x1ff) << 16) |
				      (data & 0xffff));
		}
		return;

	case AVAGO_ESB:
		if (avago_get_spico_running_flag(ah, addr) ||
		    avago_set_spico_running_flag(ah, addr,
						 avago_spico_running(ah,
								     addr))) {
			avago_spico_int_check(ah, __func__, __LINE__, addr,
					      0x18, 0x4000 | (mem_addr & 0x3fff));
			avago_spico_int_check(ah, __func__, __LINE__, addr,
					      0x19, data);
			return;
		}

	case AVAGO_ESB_DIRECT:
		{
			int lsb_esb_addr, lsb_esb_write_data;
			int spico_run_state = avago_spico_halt(ah, addr);

			int reset_status = avago_sbus_rd(ah, addr, 0x07);
			if (reset_status & 0x01)
				avago_sbus_wr(ah, addr, 0x07, reset_status & ~1);

			lsb_esb_write_data =
			    avago_serdes_mem_rd(ah, addr, AVAGO_LSB_DIRECT,
						0x31);
			lsb_esb_addr =
			    avago_serdes_mem_rd(ah, addr, AVAGO_LSB_DIRECT,
						0x30);

			if (lsb_esb_addr & 0x8000)
				avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
						    0x30,
						    lsb_esb_addr & 0x7fff);

			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x31, data);
			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x30, mem_addr);
			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x30, mem_addr | 0x8000);
			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x30, mem_addr);

			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x31, lsb_esb_write_data);
			if (lsb_esb_addr & 0x8000)
				avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
						    0x30, lsb_esb_addr & 0x7fff);
			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT,
					    0x30, lsb_esb_addr);

			if (reset_status & 0x01)
				avago_sbus_wr(ah, addr, 0x07, reset_status);
			avago_spico_resume(ah, addr, spico_run_state);
			return;
		}

	case AVAGO_DMEM:
		if (sdrev == AVAGO_SDREV_HVD6) {
			avago_sbus_wr(ah, addr, 0x01,
				      (1 << 29) | (1 << 31) |
				      ((data & 0xffff) << 12) |
				      (mem_addr & 0x03ff));
			return;
		} else {
			int spico_run_state = avago_spico_halt(ah, addr);
			int initial_value_0x01 =
			    avago_sbus_rd(ah, addr, 0x01);
			avago_sbus_wr(ah, addr, 0x01,
				      0x40000000 | (mem_addr & 0xfff) |
				      ((data & 0xffff) << 12));
			avago_sbus_wr(ah, addr, 0x01,
				      0xc0000000 | (mem_addr & 0xfff) |
				      ((data & 0xffff) << 12));
			avago_sbus_wr(ah, addr, 0x01, initial_value_0x01);
			avago_spico_resume(ah, addr, spico_run_state);
			return;
		}

	case AVAGO_IMEM:
		{
			int spico_run_state = avago_spico_halt(ah, addr);
			int initial_value_0x00 = avago_sbus_rd(ah, addr, 0x00);
			avago_sbus_wr(ah, addr, 0x00,
				      0x40000000 | (mem_addr & 0xffff) |
				      ((data & 0x03ff) << 16));
			avago_sbus_wr(ah, addr, 0x00,
				      0xc0000000 | (mem_addr & 0xffff) |
				      ((data & 0x03ff) << 16));
			avago_sbus_wr(ah, addr, 0x00, initial_value_0x00);
			avago_spico_resume(ah, addr, spico_run_state);
			return;
		}

	default:;
	}

	avago_fail(ah, __func__, __LINE__, "SBus %s, Invalid DMA type (%d).\n",
		   avago_addr_to_str(addr), type);
}

/** @brief   Performs a read-modify-write operation on SerDes memory location. */
/** */
/** @return  Returns the value of the memory location before it was modified. */
/** @return  On error, decrements ah->return_code. */
/** @see     avago_serdes_mem_rd(), avago_serdes_mem_wr(). */
int avago_serdes_mem_rmw(struct avago_hdl *ah,	/**< [in] Pointer to struct avago_hdl structure. */
			 uint32_t addr,	    /**< [in] Device address number. */
			 enum avago_serdes_mem_type type,
					/**< [in] The memory access type. */
			 uint32_t mem_addr, /**< [in] Memory address to access. */
			 uint32_t data,	    /**< [in] Data to write. */
			 uint32_t mask)
{
/**< [in] Bits in existing data to modify. */
	int initial_value;
	int spico_run_state = avago_spico_halt(ah, addr);

	if (type == AVAGO_LSB)
		type = AVAGO_LSB_DIRECT;
	else if (type == AVAGO_ESB)
		type = AVAGO_ESB_DIRECT;

	initial_value = avago_serdes_mem_rd(ah, addr, type, mem_addr);
	avago_serdes_mem_wr(ah, addr, type, mem_addr,
			    (data & mask) | (initial_value & ~mask));

	avago_spico_resume(ah, addr, spico_run_state);
	return initial_value;
}

static int serdes_get_int01_bits(struct avago_hdl *ah, uint32_t addr,
				 uint32_t mask)
{
	int bits = 0;
	int reg_serdes_rdy, reg_accel_misc_ctl, bit;
	switch (avago_get_sdrev(ah, addr)) {
	default:
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
	case AVAGO_SDREV_OM4:
	case AVAGO_SDREV_16:
		reg_serdes_rdy = 0x26;
		reg_accel_misc_ctl = 0xdf;
		bit = 0;
		break;
	case AVAGO_SDREV_HVD6:
		reg_serdes_rdy = 0x26;
		reg_accel_misc_ctl = 0xde;
		bit = 0;
		break;
	case AVAGO_SDREV_D6:
		reg_serdes_rdy = 0x26;
		reg_accel_misc_ctl = 0x213;
		bit = 1;
		break;
	case AVAGO_SDREV_P1:
		reg_serdes_rdy = 0xee;
		reg_accel_misc_ctl = 0x15e;
		bit = 0;
		if (avago_get_process_id(ah, addr) == AVAGO_PROCESS_B)
			reg_accel_misc_ctl = 0x162;
		break;
	}
	if (mask & 0x03) {
		bits |= 0x03 & avago_serdes_mem_rd(ah, addr, AVAGO_LSB,
						   reg_serdes_rdy);
	}
	if (mask & 0x04) {
		int reg = avago_serdes_mem_rd(ah, addr, AVAGO_ESB,
					      reg_accel_misc_ctl) >> bit;
		bits |= (0x01 & reg) << 2;
	}
	if (mask & 0x08)
		bits |= 0;
	if (mask & 0x10)
		bits |= 0;
	if (mask & 0x20)
		bits |= 0;
	if (mask & 0x40)
		bits |= 0;
	if (mask & 0x80)
		bits |= 0;
	return bits;
}

/** @brief   Gets the TX & RX ready states. */
/** @details Returns if the TX and RX devices are initialized and ready */
/**          for operation. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_output_enable(), avago_serdes_set_tx_output_enable(), avago_serdes_set_tx_rx_enable(). */
int avago_serdes_get_tx_rx_ready(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,
				/**< [in] Device address number. */
				 bool * tx,
			    /**< [out] Where to store TX state. */
				 bool * rx)
{
/**< [out] Where to store RX state. */
	int return_code = ah->return_code;
	int bits = serdes_get_int01_bits(ah, addr, 0x03);
	*tx = (bits & 0x01) == 0x01;
	*rx = (bits & 0x02) == 0x02;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, TX %s, RX %s\n",
			 avago_addr_to_str(addr),
			 avago_enable_to_str(*tx),
			 avago_enable_to_str(*rx));
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Sets TX & RX enable states, and TX output values. */
/** @details If enabling TX or RX, waits for the device to become ready. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @return  On timeout, returns -1 and does not decrement ah->return_code. */
/** @see     avago_serdes_get_tx_output_enable(), avago_serdes_get_tx_rx_ready(), avago_serdes_set_tx_output_enable(). */
int avago_serdes_set_tx_rx_enable(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				  uint32_t addr,
				/**< [in] Device address number. */
				  bool tx,
			    /**< [in] New TX state. */
				  bool rx,
			    /**< [in] New RX state. */
				  bool tx_output)
{
/**< [in] New TX output state. */
	struct avago_addr addr_struct;
	int return_code = ah->return_code;
	int loops = 0;
	int mask;

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, tx_en=%s, rx_en=%s,tx_output_en=%s\n",
			 avago_addr_to_str(addr), avago_bool_to_str(tx),
			 avago_bool_to_str(rx), avago_bool_to_str(tx_output));

	mask = serdes_get_int01_bits(ah, addr, ~0x07);
	if (tx)
		mask |= 0x01;
	if (rx)
		mask |= 0x02;
	if (tx_output)
		mask |= 0x04;

	if (!avago_spico_int_check(ah, __func__, __LINE__, addr, 0x01, mask))
		return -1;

	mask &= 0x03;
	avago_addr_to_struct(addr, &addr_struct);
	for (loops = 0; loops <= AVAGO_SERDES_INIT_RDY_TIMEOUT; loops++) {
		struct avago_addr start, stop, next;
		bool st;
		int not_ok = 0;
		if (loops > 0)
			mdelay(1);

		for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop,
						&next, 0);
		     st;
		     st = avago_broadcast_next(ah, &next, &start, &stop, 0)) {
			uint32_t lane_addr = avago_struct_to_addr(&next);

			if ((serdes_get_int01_bits(ah, lane_addr, mask) & mask)
			    != mask)
				not_ok++;
		}
		if (not_ok == 0) {
			if( loops > 0 )
				AVAGO_LOG_PRINT6(ah, AVAGO_DEBUG6, __func__,
						 __LINE__,
						 "SBus %s, Int 0x01 loops: %d\n",
						 avago_addr_to_str(addr), loops);
			return return_code == ah->return_code ? 0 : -1;
		}
	}
	avago_log_printf(ah, AVAGO_WARNING, __func__, __LINE__,
			 "SBus %s, TX/RX RDY check timed out after %d loops. mask: 0x%x\n",
			 avago_addr_to_str(addr), loops, mask);

	return -1;
}

/** @brief   Gets the TX output enable state. */
/** @details */
/** @return  Returns true if TX output is enabled, false otherwise. */
/** @see     avago_serdes_get_tx_rx_ready(), avago_serdes_set_tx_output_enable(), avago_serdes_set_tx_rx_enable(). */
bool avago_serdes_get_tx_output_enable(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				       uint32_t addr)
{
/**< [in] Device address number. */
	bool ret = serdes_get_int01_bits(ah, addr, 0x04) == 0x04;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, TX output %s\n",
			 avago_addr_to_str(addr),
			 avago_enable_to_str(ret));
	return ret;
}

/** @brief   Sets the TX output enable state. */
/** @details Turns on TX if not already on. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_output_enable(), avago_serdes_get_tx_rx_ready(), avago_serdes_set_tx_rx_enable(). */
int avago_serdes_set_tx_output_enable(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				      uint32_t addr,
					/**< [in] Device address number. */
				      bool enable)
{
/**< [in] true to enable output signal. */
	int return_code = ah->return_code;
	int mask;

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, TX output_en=%s\n",
			 avago_addr_to_str(addr),
			 avago_bool_to_str(enable));
	mask = serdes_get_int01_bits(ah, addr, ~4) | (enable ? 4 : 0);
	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x01, mask);
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Helper function to retry spico int 0x02 if it times out. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
static int spico_int_02_retry(struct avago_hdl *ah, uint32_t addr, int data,
			      int retry)
{
	int i;
	for (i = 0; i < retry; i++) {
		if (0x02 == (0xff & avago_spico_int(ah, addr, 0x02, data)))
			return 0;
		avago_sbus_rd(ah, addr, 0x04);
	}
	printf("ERROR: (%s:%d): SBus %x, PRBS reconfigure timed out.\n",
	       __func__, __LINE__, addr);

	return -1;
}

/** @brief   Gets the selected TX data source. */
/** */
/** @return  Returns the selected TX data source. */
/** @return  On error, ah->return_code is set negative. */
/** @see     avago_serdes_set_tx_data_sel(). */
enum avago_serdes_tx_data_sel avago_serdes_get_tx_data_sel(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
							   uint32_t addr)
{
/**< [in] Device address number. */
	enum avago_serdes_tx_data_sel ret = AVAGO_SERDES_TX_DATA_SEL_CORE;
	uint32_t reg_tx_data, reg_lb_data, lb_bit, reg_tx_prbs_control = 0x29;
	int data;

	int sdrev = avago_get_sdrev(ah, addr);
	switch (sdrev) {
	default:
	case AVAGO_SDREV_16:
		reg_tx_data = 0x21;
		reg_tx_prbs_control = 0x29;
		reg_lb_data = 0xdf;
		lb_bit = 1 << 2;
		break;
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
	case AVAGO_SDREV_OM4:
		reg_tx_data = 0x21;
		reg_tx_prbs_control = 0x29;
		reg_lb_data = 0xf1;
		lb_bit = 1 << 0;
		break;
	case AVAGO_SDREV_HVD6:
		reg_tx_data = 0x21;
		reg_tx_prbs_control = 0x29;
		reg_lb_data = 0xde;
		lb_bit = 1 << 2;
		break;
	case AVAGO_SDREV_D6:
		reg_tx_data = 0x21;
		reg_tx_prbs_control = 0x29;
		reg_lb_data = 0x21;
		lb_bit = 1 << 5;
		break;
	case AVAGO_SDREV_P1:
		reg_tx_data = 0xe9;
		reg_tx_prbs_control = 0xf1;
		reg_lb_data = 0xc7;
		lb_bit = 1 << 0;
		break;
	}

	data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_lb_data);
	if (data & lb_bit) {
		ret = AVAGO_SERDES_TX_DATA_SEL_LOOPBACK;

	} else {

		if (reg_lb_data != reg_tx_data)
			data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB,
						   reg_tx_data);
		if (data & (1 << 4)) {
			data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB,
						   reg_tx_prbs_control);
			ret = (enum avago_serdes_tx_data_sel)(data & 0x07);
		}
	}
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, data_sel=%s\n",
			 avago_addr_to_str(addr),
			 avago_data_sel_to_str(ret));
	return ret;
}

/** @brief   Sets the TX data source. */
/** */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_data_sel(). */
int avago_serdes_set_tx_data_sel(struct avago_hdl *ah,
						    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,	/**< [in] Device address number. */
				 enum avago_serdes_tx_data_sel source)
{
/**< [in] New TX data selection. */
	int return_code = ah->return_code;
	int int_data;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, data_sel=%s\n", avago_addr_to_str(addr),
			 avago_data_sel_to_str(source));

	int_data =
	    source == AVAGO_SERDES_TX_DATA_SEL_LOOPBACK ? 0x0210 : 0x0200;
	if (!avago_spico_int_check(ah, __func__, __LINE__, addr, 0x08, int_data))
		return -1;

	switch (source) {
	case AVAGO_SERDES_TX_DATA_SEL_CORE:
		spico_int_02_retry(ah, addr, 0x01ff, 8);
		break;

	case AVAGO_SERDES_TX_DATA_SEL_PRBS7:
	case AVAGO_SERDES_TX_DATA_SEL_PRBS9:
	case AVAGO_SERDES_TX_DATA_SEL_PRBS11:
	case AVAGO_SERDES_TX_DATA_SEL_PRBS15:
	case AVAGO_SERDES_TX_DATA_SEL_PRBS23:
	case AVAGO_SERDES_TX_DATA_SEL_PRBS31:
	case AVAGO_SERDES_TX_DATA_SEL_USER:
		int_data = 0x0100 | 0x0020 | (source & 0x0007);
		spico_int_02_retry(ah, addr, int_data, 8);
		if (avago_get_lsb_rev(ah, addr) == 8) {
			int_data = 0x0100 | 0x0000 | (source & 0x0007);
			spico_int_02_retry(ah, addr, int_data, 8);
		}
		break;

	case AVAGO_SERDES_TX_DATA_SEL_LOOPBACK:
		break;
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Retrieves the 80-bit TX user data register. */
/** @details Note: Only the first 20 bits of the pattern are returned */
/**          as that is all that is readily available from the SerDes. */
/**          These bits are then duplicated into the other bits. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_set_tx_user_data(). */
int avago_serdes_get_tx_user_data(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				  uint32_t addr,
					/**< [in] Device address number. */
				  long user_data[4])
{
/**< [out] 80-bit user data (4 x 20). */
	int return_code = ah->return_code;
	int value;
	int reg_prbs_seed_lo, reg_prbs_seed_hi;

	switch (avago_get_sdrev(ah, addr)) {
	case AVAGO_SDREV_P1:
		reg_prbs_seed_lo = 0xf4;
		reg_prbs_seed_hi = 0xf5;
		break;
	default:
	case AVAGO_SDREV_16:
		reg_prbs_seed_lo = 0x2c;
		reg_prbs_seed_hi = 0x2d;
		break;
	}

	value = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_prbs_seed_lo);
	value |=
	    (avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_prbs_seed_hi) & 0x0f)
	    << 16;
	user_data[0] = user_data[1] = user_data[2] = user_data[3] =
	    value & 0x000fffff;
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Loads the 80-bit value into the TX user data register */
/**          and select it as the TX input. */
/** @details The bit transmit order is from the least significant bit [0] of */
/**          user_data[0] through bit [19] of user_data[3]. */
/**          Each word holds 20 significant bits. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_user_data(). */
int avago_serdes_set_tx_user_data(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				  uint32_t addr,
					/**< [in] Device address number. */
				  long user_data[4])
{
/**< [in] 80-bit user data (4 x 20). */
	int return_code = ah->return_code;
	int i;

	if (!avago_spico_int_check(ah, __func__, __LINE__, addr, 0x0018, 0x0000))
		return -1;

	for (i = 0; i < 4; i++) {
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x0019,
				      user_data[i] & 0x03ff);
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x0019,
				      (user_data[i] >> 10) & 0x03ff);
	}
	if (ah->return_code != return_code)
		return -1;
	return avago_serdes_set_tx_data_sel(ah, addr,
					    AVAGO_SERDES_TX_DATA_SEL_USER);
}

/** @brief   Gets the TX inverter polarity. */
/** @return  Returns true if inverter is enabled, false if data is not being inverted. */
/** @return  On error, decrements ah->return_code. */
/** @see     avago_serdes_get_rx_invert(). */
/** @see     avago_serdes_set_tx_invert(). */
bool avago_serdes_get_tx_invert(struct avago_hdl * ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				uint32_t addr)
{
/**< [in] Device address number. */
	int ret;
	int reg_invert, invert_bit;
	switch (avago_get_sdrev(ah, addr)) {
	default:
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
	case AVAGO_SDREV_OM4:
	case AVAGO_SDREV_16:
		reg_invert = 0xdf;
		invert_bit = 1 << 5;
		break;
	case AVAGO_SDREV_P1:
		reg_invert = 0x15e;
		invert_bit = 1 << 5;
		if (avago_get_process_id(ah, addr) == AVAGO_PROCESS_B) {
			reg_invert = 0x162;
			invert_bit = 7 << 7;
		}
		break;
	case AVAGO_SDREV_HVD6:
		reg_invert = 0xde;
		invert_bit = 1 << 5;
		break;
	case AVAGO_SDREV_D6:
		reg_invert = 0x211;
		invert_bit = 1 << 3;
		break;
	}
	ret = !!(avago_serdes_mem_rd(ah, addr, AVAGO_ESB, reg_invert) &
		 invert_bit);
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, tx_invert=%s\n", avago_addr_to_str(addr),
			 avago_onoff_to_str(ret));
	return ret;
}

/** @brief   Sets the TX inverter polarity. */
/** @details Set invert to true to enable the inverter, false to not invert the data. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_invert(). */
/** @see     avago_serdes_set_rx_invert(). */
int avago_serdes_set_tx_invert(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
			       uint32_t addr,
					/**< [in] Device address number. */
			       bool invert)
{
/**< [in] true to invert output data. */

	uint32_t data = invert ? 0x0101 : 0x0100;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, tx_invert=%s\n", avago_addr_to_str(addr),
			 avago_onoff_to_str(invert));
	if (!avago_spico_int_check(ah, __func__, __LINE__, addr, 0x13, data))
		return -1;
	return 0;
}

/** @brief   Retrieves the Tx data path configuration values
 * * @return  On success, returns 0.
 ** @return  On error, decrements ah->return_code and returns -1.
 ** @see avago_serdes_get_rx_datapath_encoding().
 ** @see avago_serdes_set_tx_datapath_encoding().
 **/
int avago_serdes_get_tx_datapath_encoding(
	struct avago_hdl *ah,     /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t addr,                  /**< [in] Device address number. */
	bool *gray_encode,          /**< [out] Returns true if gray encoding is enabled. */
	bool *precode)              /**< [out] Returns true if precode encoding is enabled. */
{
	int return_code = ah->return_code;
	*gray_encode = *precode = false;
	if (avago_serdes_get_tx_line_encoding(ah, addr)) {
		int data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x29);
		*gray_encode = 0 != (data & (1<<7));

		data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x21);
		*precode = 0 != (data & (1<<9));
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Configures the data path encoding modes for the Tx.
 * * @details Does nothing unless device is an M4 SerDes in PAM4 mode.
 ** @return  On success, returns 0.
 ** @return  On error, decrements ah->return_code and returns -1.
 ** @see avago_serdes_get_tx_datapath_encoding().
 ** @see avago_serdes_set_rx_datapath_encoding().
 **/
int avago_serdes_set_tx_datapath_encoding(
	struct avago_hdl *ah,               /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t addr,                  /**< [in] Device address number. */
	bool gray_encode,           /**< [in] true to enable gray encoding (PAM4 only) */
	bool precode)               /**< [in] true to enable precode encoding (PAM4 only). */
{
	int return_code = ah->return_code;
	if (avago_serdes_get_tx_line_encoding(ah,addr))
	{
		int data = 0x400;
		data |= gray_encode ? 2 : 0;
		data |= precode ? 4 : 0;
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x13, data);
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Retrieves the Rx data path configuration values
 * * @return  On success, returns 0.
 ** @return  On error, decrements ah->return_code and returns -1.
 ** @see avago_serdes_get_tx_datapath_encoding().
 ** @see avago_serdes_set_rx_datapath_encoding().
 **/
int avago_serdes_get_rx_datapath_encoding(
	struct avago_hdl *ah,               /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t addr,                  /**< [in] Device address number. */
	bool *gray_decode,          /**< [out] Returns true if gray decoding is enabled. */
	bool *predecode)            /**< [out] Returns true if precode decoding is enabled. */
{
	int return_code = ah->return_code;
	*gray_decode = *predecode = false;
	if (avago_serdes_get_rx_line_encoding(ah,addr)) {
		int data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x2b);
		*gray_decode = 0 != (data & (1<<3));

		data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x25);
		*predecode = 0 != (data & (1<<4));
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Configures the data path encoding modes for the Rx.
 * * @details Does nothing unless device is an M4 SerDes in PAM4 mode.
 ** @return  On success, returns 0.
 ** @return  On error, decrements ah->return_code and returns -1.
 ** @see avago_serdes_get_rx_datapath_encoding().
 ** @see avago_serdes_set_tx_datapath_encoding().
 **/
int avago_serdes_set_rx_datapath_encoding(
	struct avago_hdl *ah,               /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t addr,                  /**< [in] Device address number. */
	bool gray_decode,           /**< [in] true to enable gray decoding (PAM4 only). */
					  bool predecode)             /**< [in] true to enable precode decoding (PAM4 only). */
{
	int return_code = ah->return_code;
	if (avago_serdes_get_rx_line_encoding(ah,addr)) {
		int data = 0x800;
		data |= gray_decode ? 0x20 : 0;
		data |= predecode ? 0x40 : 0;
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x13, data);
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @cond INTERNAL */
static const uint8_t width_tab[] = { 10, 16, 20, 32, 40, 64, 80, 80 };

int avago_serdes_bits_to_width(uint32_t bits)
{
	return width_tab[bits & 7];
}
static const uint8_t width_val[] = { 0, 4, 2, 5, 3, 6, 7, 7 };

int avago_serdes_width_to_bits(int width)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(width_tab); i++)
		if (width_tab[i] == width)
			return width_val[i];
	return -1;
}

/** @endcond */

/** @brief   Gets the TX and RX data widths. */
/** @details Typically, these are set to the same value, but can be different */
/**          for specialized situations. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_set_tx_rx_width(). */
int avago_serdes_get_tx_rx_width(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,
				/**< [in] Device address number. */
				 int *tx,
			    /**< [out] Where to store TX width. */
				 int *rx)
{
/**< [out] Where to store RX width. */
	int return_code = ah->return_code;
	int data, sdrev = avago_get_sdrev(ah, addr);
	uint32_t tx_bits, rx_bits, tx_mul = 1, rx_mul = 1;
	enum avago_serdes_mem_type reg_type;
	int reg_width, tx_shift, rx_shift;
	switch (sdrev) {
	default:
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
	case AVAGO_SDREV_OM4:
	case AVAGO_SDREV_16:
	case AVAGO_SDREV_HVD6:
		reg_width = 0x22;
		reg_type = AVAGO_LSB;
		tx_shift = 8, rx_shift = 12;
		break;
	case AVAGO_SDREV_P1:
		reg_width = 0xea;
		reg_type = AVAGO_LSB;
		tx_shift = 8;
		rx_shift = 12;
		break;
	case AVAGO_SDREV_D6:
		reg_width = 0x301;
		reg_type = AVAGO_ESB;
		tx_shift = 0;
		rx_shift = 3;
		break;
	}

	data = avago_serdes_mem_rd(ah, addr, reg_type, reg_width);

	tx_bits = (data >> tx_shift) & 0x7;
	rx_bits = (data >> rx_shift) & 0x7;

	if (sdrev == AVAGO_SDREV_D6) {
		tx_bits &= 6;
		rx_bits &= 6;
	}
	if (sdrev == AVAGO_SDREV_16 || sdrev == AVAGO_SDREV_HVD6)
		tx_mul +=
		    ((avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x21) >> 3) &
		     0x1);
	*tx = avago_serdes_bits_to_width(tx_bits) * tx_mul;

	if (sdrev != AVAGO_SDREV_D6 && sdrev != AVAGO_SDREV_P1)
		rx_mul +=
		    ((avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x24) >> 3) &
		     0x1);
	*rx = avago_serdes_bits_to_width(rx_bits) * rx_mul;

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, tx/rx width mode %d/%d bits\n",
			 avago_addr_to_str(addr), *tx, *rx);
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Sets the TX and RX data width. */
/** @details Supported widths are hardware and rate dependent. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_rx_width(). */
int avago_serdes_set_tx_rx_width(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,
				/**< [in] Device address number. */
				 int tx,
			    /**< [in] New TX width. */
				 int rx)
{
/**< [in] New RX width. */
	return avago_serdes_set_tx_rx_width_pam(ah, addr, tx, rx,
						AVAGO_SERDES_NRZ,
						AVAGO_SERDES_PAM2);
}

/**
 * @brief   Sets the TX and RX data width.
 *
 * @param	ah	Avago handle
 * @param	addr	Device address number
 * @param	tx	New Tx width
 * @param	rx	New Rx width
 * @param	tx_encoding	Transmit serial line encoding
 * @param	rx_encoding	Receive serial line encoding
 *
 * @details Supported widths are hardware and rate dependent.
 * @return  On success, returns 0.
 * @return  On error, decrements ah->return_code and returns -1.
 * @see     avago_serdes_get_tx_rx_width().
 */
int avago_serdes_set_tx_rx_width_pam(struct avago_hdl *ah,
				     uint32_t addr,
				     int tx,
				     int rx,
				     enum avago_serdes_line_encoding tx_encoding,
				     enum avago_serdes_line_encoding rx_encoding)
{
	int return_code = ah->return_code;
	int build_id = avago_firmware_get_build_id(ah, addr);
	int sdrev = avago_get_sdrev(ah, addr);
	int bits = avago_serdes_width_to_bits(tx) |
		   (avago_serdes_width_to_bits(rx) << 4);
	if (bits < 0) {
		return avago_fail(ah, __func__, __LINE__,
				  "ERROR: SBus %s, invalid width(s) %d/%d requested.\n",
				  avago_addr_to_str(addr), tx, rx);

	}
	if ((build_id & 2) == 0)
		bits |= 0x8800;
	if (sdrev == AVAGO_SDREV_OM4 || sdrev == AVAGO_SDREV_CM4 ||
	    sdrev == AVAGO_SDREV_CM4_16) {
		if (tx_encoding == AVAGO_SERDES_PAM4)
			bits |= 0x08;
		if (rx_encoding == AVAGO_SERDES_PAM4)
			bits |= 0x80;
	} else if (sdrev == AVAGO_SDREV_P1) {
		if (tx != rx)
			return avago_fail(ah, __func__, __LINE__,
					  "SBus %s: SerDes P1 requires Tx and Rx widths to match.\n",
					  avago_addr_to_str(addr));
	}
	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x14, bits);
	return return_code == ah->return_code ? 0 : -1;
}

/**
 * @brief   Retrieves the Tx and Rx line encoding values.
 *
 * @param	ah	Avago handle
 * @param	addr	Device address number
 * @param[out]	tx	Transmit serial line encoding
 * @param[out]	rx	Receive serial line encoding
 *
 * @return  On success, returns 0.
 * @return  On error, decrements ah->return_code and returns -1.
 */
int avago_serdes_get_tx_rx_line_encoding(struct avago_hdl *ah, uint32_t addr,
					 enum avago_serdes_line_encoding *tx,
					 enum avago_serdes_line_encoding *rx)
{
	int return_code = ah->return_code;
	if (avago_get_ip_type(ah, addr) == AVAGO_M4) {
		int reg = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x22);
		*tx = (reg & 0x10) ? AVAGO_SERDES_PAM4 : AVAGO_SERDES_PAM2;
		*rx = (reg & 0x40) ? AVAGO_SERDES_PAM4 : AVAGO_SERDES_PAM2;
		AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5,__func__,__LINE__,
				 "SBus %s, ip_type %d, return tx=%s, rx=%s\n",
				avago_addr_to_str(addr),
				avago_get_ip_type(ah, addr),
				avago_line_encoding_to_str(*tx),
				avago_line_encoding_to_str(*rx));
	}
	else
		*tx = *rx = AVAGO_SERDES_PAM2;
	return return_code == ah->return_code ? 0 : -1;
}


/**
 * @brief	Retrieves the Tx symbol encoding value.
 * @deprecated	This function is deprecated
 * @see avago_serdes_tx_rx_line_encoding()
 *
 * @param	ah	Avago handle
 * @param	addr	Device address number
 * @details	SerDes which do not support PAM encoding always return false.
 * @return	Returns true if the SerDes is configured for PAM4 mode and false
 *		otherwise.
 */
bool avago_serdes_get_tx_line_encoding(struct avago_hdl * ah, uint32_t addr)
{
	enum avago_serdes_line_encoding tx, rx;

	return	avago_serdes_get_tx_rx_line_encoding(ah, addr, &tx, &rx) == 0 &&
		tx == AVAGO_SERDES_PAM4;
}

/**
 * @brief   Retrieves the Rx symbol encoding value.
 * @deprecated	This function is deprecated
 * @see avago_serdes_tx_rx_line_encoding()
 * @details SerDes which do not support PAM encoding always return false.
 * @return	Returns true if the SerDes is configured for PAM4 mode and false
 *		otherwise.
 */
bool avago_serdes_get_rx_line_encoding(struct avago_hdl * ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				       uint32_t addr)
{
/**< [in] Device address number. */
enum avago_serdes_line_encoding tx, rx;

	return	avago_serdes_get_tx_rx_line_encoding(ah, addr, &tx, &rx) == 0 &&
		rx == AVAGO_SERDES_PAM4;
}

/** @brief  Initialize the structure to default values. */
/** @return void */
void avago_serdes_tx_eq_init(struct avago_serdes_tx_eq *ptr)
{
/**< Structure pointer to initialize */
	memset(ptr, 0, sizeof(struct avago_serdes_tx_eq));
}

/** @brief   Gets the TX equalization values. */
/** @details Retrieves all valid values, sets others to zero. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_set_tx_eq(), avago_serdes_get_tx_eq_limits(). */
int avago_serdes_get_tx_eq(struct avago_hdl *ah,/**< [in] Pointer to struct avago_hdl structure. */
			   uint32_t addr,   /**< [in] Device address number. */
			   struct avago_serdes_tx_eq *tx_eq)
{
/**< [in] New TX equalization values. */
	int return_code = ah->return_code;
	int reg_slew;
	int sdrev = avago_get_sdrev(ah, addr);
	avago_serdes_tx_eq_init(tx_eq);
	if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false, 1,
				     0x1043)) {
		tx_eq->pre =
		    (short)avago_spico_int(ah, addr, 0x15,
					   (1 << 8) | (0 << 14));
		tx_eq->atten =
		    (short)avago_spico_int(ah, addr, 0x15,
					   (1 << 8) | (1 << 14));
		tx_eq->post =
		    (short)avago_spico_int(ah, addr, 0x15,
					   (1 << 8) | (2 << 14));
		switch (sdrev) {
		case AVAGO_SDREV_CM4:
			tx_eq->pre2 =
			    (short)avago_spico_int(ah, addr, 0x15,
						   (1 << 8) | (3 << 14));
			tx_eq->pre3 =
			    (short)avago_spico_int(ah, addr, 0x15,
						   (1 << 8) | (0xf << 12));
			break;
		case AVAGO_SDREV_OM4:
			tx_eq->vert =
			    (short)avago_spico_int(ah, addr, 0x15,
						   (1 << 8) | (3 << 14));
			break;
		default:
			break;
		}
	} else {
		uint32_t i = avago_spico_int(ah, addr, 0x15, 1 << 8);
		tx_eq->pre = (i >> 12) & 0x0F;
		tx_eq->atten = (i >> 5) & 0x1F;
		tx_eq->post = (i >> 0) & 0x1F;
	}
	switch (sdrev) {
	case AVAGO_SDREV_D6:
		reg_slew = 0x240;
		break;
	case AVAGO_SDREV_HVD6:
		reg_slew = 0x040;
		break;
	case AVAGO_SDREV_16:
		reg_slew = 0x0e5;
		break;
	default:
	case AVAGO_SDREV_P1:
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_OM4:
		reg_slew = 0;
		tx_eq->slew = 0;
		break;
	}
	if (reg_slew)
		tx_eq->slew = (short)avago_serdes_mem_rd(ah, addr, AVAGO_ESB,
							 reg_slew) & 3;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, tx_eq=%d,%d,%d,%d\n",
			 avago_addr_to_str(addr), tx_eq->pre, tx_eq->atten,
			 tx_eq->post, tx_eq->slew);
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Sets the TX equalization values. */
/** @details Set all values, if applicable. */
/**          Note: The slew rate is not adjustable on high speed SerDes and */
/**                must be set to 0 in this case. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_eq(), avago_serdes_get_tx_eq_limits(). */
int avago_serdes_set_tx_eq(struct avago_hdl *ah,/**< [in] Pointer to struct avago_hdl structure. */
			   uint32_t addr,   /**< [in] Device address number. */
			   struct avago_serdes_tx_eq *tx_eq)
{
/**< [in] New set of values. */
	int return_code = ah->return_code;
	int rc;
	int sdrev = avago_get_sdrev(ah, addr);
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, tx_eq=%d,%d,%d\n",
			 avago_addr_to_str(addr), tx_eq->pre, tx_eq->atten,
			 tx_eq->post);

	switch (sdrev) {
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
		rc = 0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						     (3 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (2 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (1 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (0 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (0xf << 12) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (3 << 14) |
						    (tx_eq->pre2 & 0xFF))) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (2 << 14) |
						    (tx_eq->post & 0xFF))) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (1 << 14) |
						    (tx_eq->atten & 0xFF))) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (0 << 14) |
						    (tx_eq->pre & 0xFF))) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (0xf << 12) |
						    (tx_eq->pre3 & 0xFF)));
		if (!rc)
			avago_fail(ah, __func__, __LINE__,
				   "SBus %s, Invalid TX equalization set: pre1=%d, pre2=%d, pre3=%d, atten=%d, post=%d.\n",
				   avago_addr_to_str(addr), tx_eq->pre,
				   tx_eq->pre2, tx_eq->pre3, tx_eq->atten,
				   tx_eq->post);
		break;
	case AVAGO_SDREV_OM4:

		rc = 0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						     (3 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (2 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (1 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (0 << 14) | 0)) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (3 << 14) |
						    (tx_eq->vert & 0xFF))) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (2 << 14) |
						    (tx_eq->post & 0xFF))) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (1 << 14) |
						    (tx_eq->atten & 0xFF))) &&
		    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
						    (0 << 14) |
						    (tx_eq->pre & 0xFF)));
		    if (!rc)
			    avago_fail(ah, __func__, __LINE__,
				       "SBus %s, Invalid TX equalization set: pre=%d, atten=%d, post=%d, vert=%d.\n",
				       avago_addr_to_str(addr), tx_eq->pre,
				       tx_eq->atten, tx_eq->post, tx_eq->vert);
			    break;
	default:
		if (tx_eq->atten == tx_eq->pre && tx_eq->atten == tx_eq->post) {
			rc = 0xff & avago_spico_int(ah, addr, 0x15,
						    (3 << 14) |
						    (tx_eq->atten & 0xFF));
		} else {
			rc = 0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
							     (3 << 14) | 0)) &&
			    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
							    (2 << 14) |
							    (tx_eq->post & 0xFF))) &&
			    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
							    (1 << 14) |
							    (tx_eq->atten & 0xFF))) &&
			    0x15 == (0xff & avago_spico_int(ah, addr, 0x15,
							    (0 << 14) |
							    (tx_eq->pre & 0xFF)));
		}
		if (!rc)
			avago_fail(ah, __func__,__LINE__,
				   "SBus %s, Invalid TX equalization set: pre=%d, atten=%d, post=%d.\n",
				   avago_addr_to_str(addr), tx_eq->pre,
				   tx_eq->atten, tx_eq->post);
		break;
	}
	if (rc) {
		int reg_slew = 0;
		switch (sdrev) {
		case AVAGO_SDREV_16:
			reg_slew = 0x0e5;
			break;
		case AVAGO_SDREV_HVD6:
			reg_slew = 0x040;
			break;
		case AVAGO_SDREV_D6:
			reg_slew = 0x240;
			break;
		default:
			reg_slew = 0;
			break;
		}
		if (reg_slew) {
			if (tx_eq->slew > 3 || tx_eq->slew < 0)
				avago_fail(ah, __func__, __LINE__,
					   "SBus %s, Invalid slew value: %d, valid range [0..3].\n",
					   avago_addr_to_str(addr), tx_eq->slew);
			else
				avago_serdes_mem_rmw(ah, addr, AVAGO_ESB,
						     reg_slew, tx_eq->slew, 3);
		}
	} else {
		int reg_slew = 0;
		switch (avago_get_sdrev(ah, addr)) {
		case AVAGO_SDREV_16:
			reg_slew = 0x0e5;
			break;
		case AVAGO_SDREV_HVD6:
			reg_slew = 0x040;
			break;
		case AVAGO_SDREV_D6:
			reg_slew = 0x240;
			break;
		default:
			reg_slew = 0;
			break;
		}
		if (reg_slew) {
			if (tx_eq->slew > 3 || tx_eq->slew < 0)
				printf("ERROR: (%s:%d): SBus %x, Invalid slew value: %d, valid range [0..3].\n",
				       __func__, __LINE__, addr, tx_eq->slew);
			else
				avago_serdes_mem_rmw(ah, addr, AVAGO_ESB,
						     reg_slew, tx_eq->slew, 3);
		}
	}

	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Gets the RX compare data configuration. */
/** */
/** @return  enum avago_serdes_rx_cmp_data value. */
/**          On error, ah->return_code is set negative. */
/** @see     avago_serdes_set_rx_cmp_data(). */
enum avago_serdes_rx_cmp_data avago_serdes_get_rx_cmp_data(struct avago_hdl *ah,
					/**< [in] Pointer to struct avago_hdl structure. */
							   uint32_t addr)
{
/**< [in] Device address number. */
	enum avago_serdes_rx_cmp_data return_data;
	uint32_t data;
	int reg_prbs_sel;
	switch (avago_get_sdrev(ah, addr)) {
	case AVAGO_SDREV_P1:
		reg_prbs_sel = 0xf2;
		break;
	default:
	case AVAGO_SDREV_D6:
		reg_prbs_sel = 0x2a;
		break;
	}
	data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_prbs_sel);
	return_data = (data & 0x8000)
	    ? (enum avago_serdes_rx_cmp_data)(data & 0x07)
	    : AVAGO_SERDES_RX_CMP_DATA_OFF;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, cmp_data=%s\n", avago_addr_to_str(addr),
			 avago_cmp_data_to_str(return_data));
	return return_data;
}

/** @brief   Sets the RX compare data configuration. */
/** @details Sets the PRBS pattern to compare incoming data against. */
/**          If AVAGO_SERDES_RX_CMP_DATA_SELF_SEED is selected, the comparator */
/**          automatically synchronizes to an incoming 80 bit pattern. */
/**          The error counter then reports deviations from that pattern. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_rx_cmp_data(). */
int avago_serdes_set_rx_cmp_data(struct avago_hdl *ah,
						    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,	/**< [in] Device address number. */
				 enum avago_serdes_rx_cmp_data data)
{
/**< [in] New RX compare data setting. */
	int int_data = 0x0200 | 0x0020 | 0x0010 | (data & 0x0007);
	if (data == AVAGO_SERDES_RX_CMP_DATA_OFF)
		int_data = 0x2ff;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, cmp_data=%s\n", avago_addr_to_str(addr),
			 avago_cmp_data_to_str(data));

	if (avago_get_lsb_rev(ah, addr) == 8) {
		spico_int_02_retry(ah, addr, int_data, 8);
		int_data = 0x0200 | 0x0000 | 0x0010 | (data & 0x0007);
		if (data == AVAGO_SERDES_RX_CMP_DATA_OFF)
			int_data = 0x2ff;
		AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
				 "SBus %s, cmp_data=%s\n",
				 avago_addr_to_str(addr),
				 avago_cmp_data_to_str(data));
	}
	return spico_int_02_retry(ah, addr, int_data, 8);
}

/** @brief   Gets the RX termination. */
/** */
/** @return  One of AVAGO_SERDES_RX_TERM_AGND, AVAGO_SERDES_RX_TERM_AVDD, or */
/**          AVAGO_SERDES_RX_TERM_FLOAT. */
/** @return  On error, decrements ah->return_code. */
/** @see     avago_serdes_set_rx_term(). */
enum avago_serdes_rx_term avago_serdes_get_rx_term(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
						   uint32_t addr)
{
/**< [in] Device address number. */
	enum avago_serdes_rx_term term = AVAGO_SERDES_RX_TERM_AGND;
	int reg_float, float_bit, reg_term, term_bit;
	int data;

	switch (avago_get_sdrev(ah, addr)) {
	default:
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
	case AVAGO_SDREV_OM4:
	case AVAGO_SDREV_16:
		reg_float = 0xc7;
		float_bit = 1 << 6;
		reg_term = 0xc7;
		term_bit = 1 << 5;
		break;
	case AVAGO_SDREV_P1:
		reg_float = 0xec;
		float_bit = 1 << 14;
		reg_term = 0xa0;
		term_bit = 1 << 5;
		break;
	case AVAGO_SDREV_HVD6:
		reg_float = 0x23;
		float_bit = 1 << 11;
		reg_term = 0x20;
		term_bit = 1 << 10;
		break;
	case AVAGO_SDREV_D6:
		reg_float = 0x24;
		float_bit = 1 << 10;
		reg_term = 0x20;
		term_bit = 1 << 10;
		break;
	}

	data = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_float);
	if (data & float_bit) {
		term = AVAGO_SERDES_RX_TERM_FLOAT;
	} else {
		if (reg_float != reg_term)
			data =
			    avago_serdes_mem_rd(ah, addr, AVAGO_ESB, reg_term);
		if (data & term_bit)
			term = AVAGO_SERDES_RX_TERM_AVDD;
	}
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, term=%s\n", avago_addr_to_str(addr),
			 avago_term_to_str(term));
	return term;
}

/** @brief   Sets the RX termination. */
/** @details Term is AVAGO_SERDES_RX_TERM_AGND, AVAGO_SERDES_RX_TERM_AVDD, or */
/**          AVAGO_SERDES_RX_TERM_FLOAT. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_rx_term(). */
int avago_serdes_set_rx_term(struct avago_hdl *ah,
						/**< [in] Pointer to struct avago_hdl structure. */
			     uint32_t addr, /**< [in] Device address number. */
			     enum avago_serdes_rx_term term)
{
/**< [in] New termination value. */
	int return_code = ah->return_code;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, term=0x%x\n", avago_addr_to_str(addr),
			 term);
	if (avago_get_lsb_rev(ah, addr) >= 3) {
		int int_data = 0;
		switch (term) {
		case AVAGO_SERDES_RX_TERM_AGND:
			int_data = 0;
			break;
		case AVAGO_SERDES_RX_TERM_AVDD:
			int_data = 1;
			break;
		case AVAGO_SERDES_RX_TERM_FLOAT:
			int_data = 2;
			break;
		}
		if (!avago_spico_int_check
		    (ah, __func__, __LINE__, addr, 0x2b, int_data))
			return -1;
	} else if (term == AVAGO_SERDES_RX_TERM_FLOAT) {
		avago_serdes_mem_rmw(ah, addr, AVAGO_LSB, 0x024, 0x0400, 0x0400);
	} else {

		int value =
		    (term == AVAGO_SERDES_RX_TERM_AVDD) ? 0x0400 : 0x0000;
		avago_serdes_mem_rmw(ah, addr, AVAGO_ESB, 0x020, value, 0x0400);
		avago_serdes_mem_rmw(ah, addr, AVAGO_LSB, 0x024, 0x0000, 0x0400);
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Reads 16 bits from the RX data stream. */
/** @details The data is read asynchronously from a live buffer, so is not */
/**          guaranteed to be consistent. */
/** @details Note: If the Rx divider is a multiple of the bit width, then */
/**          timing relationships may occur such that this call will always */
/**          read while the data is being updated.  The result is the returned */
/**          data will be consistently invalid. */
/** @return  On success, returns 16 bits of data (a positive value). */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_rx_data(). */
int avago_serdes_get_rx_live_data(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				  uint32_t addr)
{
/**< [in] Device address number. */
	int return_code = ah->return_code;
	int rx_data_obs_lo_reg =
	    avago_get_sdrev(ah, addr) == AVAGO_SDREV_P1 ? 0xf6 : 0x2e;
	int results = avago_serdes_mem_rd(ah, addr, AVAGO_LSB,
					  rx_data_obs_lo_reg) & 0xffff;
	return return_code == ah->return_code ? results : -1;
}

/** @brief   Injects errors into the RX data. */
/** @details RX error injection affects only the main data channel. */
/**          Therefore, errors are only detected if the MAIN channel */
/**          is one of the selected compare inputs. */
/** @details RX injected errors are detected in XOR and MAIN_PATGEN compare */
/**          modes, but are not detected in TEST_PATGEN mode. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_tx_inject_error(). */
int avago_serdes_rx_inject_error(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,
					/**< [in] Device address number. */
				 uint32_t num_bits)
{
/**< [in] Number of error bits to inject. */
	int sdrev = avago_get_sdrev(ah, addr);
	int bits, orig, return_code = ah->return_code;
	uint32_t mem_addr =
	    (avago_get_ip_type(ah, addr) == AVAGO_P1) ? 0xf3 : 0x2b;
	if (avago_get_lsb_rev(ah, addr) < 3) {
		return avago_fail(ah, __func__, __LINE__,
				  "ERROR: LSB revisions 01 & 02 don't support RX ERROR injection.\n");
	}

	orig = bits = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, mem_addr);

	if (sdrev == AVAGO_SDREV_HVD6) {
		bits &= ~0x07f0;
		bits |= 0x0130;
	} else if (sdrev == AVAGO_SDREV_16 || sdrev == AVAGO_SDREV_P1) {
		bits &= ~0x07f0;
		bits |= 0x0270;
	} else if (sdrev == AVAGO_SDREV_CM4_16 || sdrev == AVAGO_SDREV_CM4) {
		bits &= ~0x07f0;
		if (avago_serdes_get_rx_line_encoding(ah, addr))
			bits |= 0x04f0;
		else
			bits |= 0x04e0;
	}
	for (; num_bits > 0; num_bits--) {
		avago_serdes_mem_wr(ah, addr, AVAGO_LSB, mem_addr, bits & ~2);
		avago_serdes_mem_wr(ah, addr, AVAGO_LSB, mem_addr, bits |= 2);
	}
	if (sdrev == AVAGO_SDREV_16 || sdrev == AVAGO_SDREV_HVD6)
		avago_serdes_mem_wr(ah, addr, AVAGO_LSB, mem_addr, orig & ~2);
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Injects errors into the TX data. */
/** @details TX error injection affects the transmitted data stream. Therefore, */
/**          errors are only detected when comparing with a known pattern. */
/**          TX injected errors will be detected in RX TEST_PATGEN and MAIN_PATGEN modes. */
/**          TX injected errors will not be detected in XOR mode. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_rx_inject_error(). */
int avago_serdes_tx_inject_error(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,
					/**< [in] Device address number. */
				 uint32_t num_bits)
{
/**< [in] Number of error bits to inject (max=65535). */
	uint32_t int_data = MIN(num_bits, 0x0ffff);
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, injecting %u errors.\n",
			 avago_addr_to_str(addr), int_data);

	if (!avago_spico_int_check
	    (ah, __func__, __LINE__, addr, 0x1b, int_data))
		return -1;
	return 0;
}

/** @brief  Gets the RX compare mode. */
/** */
/** @return Returns the RX compare mode. */
/** @return On error, decrements ah->return_code. */
/** @see     avago_serdes_set_rx_cmp_mode(). */
enum avago_serdes_rx_cmp_mode avago_serdes_get_rx_cmp_mode(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
							   uint32_t addr)
{
/**< [in] Device address number. */
	int compare_reg =
	    avago_get_sdrev(ah, addr) == AVAGO_SDREV_P1 ? 0xe1 : 0x17;
	enum avago_serdes_rx_cmp_mode mode = (enum avago_serdes_rx_cmp_mode)
	    (avago_serdes_mem_rd(ah, addr, AVAGO_LSB, compare_reg) & 0x0770);
	return mode;
}

/** @brief  Sets the RX compare mode. */
/** */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_rx_cmp_mode(). */
int avago_serdes_set_rx_cmp_mode(struct avago_hdl *ah,
						    /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,	/**< [in] Device address number. */
				 enum avago_serdes_rx_cmp_mode mode)
{
/**< [in] New compare mode. */
	int return_code = ah->return_code;
	int curr_mode;
	int new_mode = 0x0003;
	int reg_compare = (avago_get_ip_type(ah, addr) == AVAGO_P1) ? 0xe1 : 0x17;

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, cmp_mode=%s\n", avago_addr_to_str(addr),
			 avago_cmp_mode_to_str(mode));
	curr_mode = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_compare);
	new_mode |= (curr_mode & ~0x0770) | (0x0770 & mode);
	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x03, new_mode);

	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Gets the RX inverter polarity. */
/** @return  Returns true if inverter is enabled, false if data is not being inverted. */
/** @return  On error, decrements ah->return_code. */
/** @see     avago_serdes_get_tx_invert(). */
/** @see     avago_serdes_set_rx_invert(). */
bool avago_serdes_get_rx_invert(struct avago_hdl * ah,
				/**< [in] Pointer to struct avago_hdl structure. */
				uint32_t addr)
{
/**< [in] Device address number. */
	bool invert;
	int reg_invert, invert_bit;
	enum avago_serdes_mem_type mem_type = AVAGO_LSB;
	switch (avago_get_sdrev(ah, addr)) {
	default:
	case AVAGO_SDREV_OM4:
	case AVAGO_SDREV_HVD6:
	case AVAGO_SDREV_16:
		reg_invert = 0x24;
		invert_bit = 1 << 0;
		break;
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
		reg_invert = 0x2b;
		invert_bit = 1 << 2;
		break;
	case AVAGO_SDREV_P1:
		reg_invert = 0xec;
		invert_bit = 1 << 0;
		break;
	case AVAGO_SDREV_D6:
		reg_invert = 0x60;
		invert_bit = 1 << 3;
		mem_type = AVAGO_ESB;
		break;
	}

	invert = !!(avago_serdes_mem_rd(ah, addr, mem_type, reg_invert) &
		    invert_bit);
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, rx_invert=%s\n", avago_addr_to_str(addr),
			 avago_onoff_to_str(invert));
	return invert;
}

/** @brief   Sets the RX inverter polarity. */
/** @details Set <b>invert</b> true to enable the inverter, false to not invert the data. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_rx_invert(). */
/** @see     avago_serdes_set_tx_invert(). */
int avago_serdes_set_rx_invert(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
			       uint32_t addr,
					/**< [in] Device address number. */
			       bool invert)
{
/**< [in] true to invert input data. */
	int data = invert ? 0x0210 : 0x0200;

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, rx_invert=%s\n", avago_addr_to_str(addr),
			 avago_onoff_to_str(invert));
	if (!avago_spico_int_check(ah, __func__, __LINE__, addr, 0x13, data))
		return -1;
	return 0;
}

/** @brief   Gets the RX signal input source. */
/** @return  Returns true if internal loopback is enabled, false if external signal. */
/** @return  On error, ah->return_code becomes negative. */
/** @see     avago_serdes_set_rx_input_loopback(). */
bool avago_serdes_get_rx_input_loopback(struct avago_hdl * ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
					uint32_t addr)
{
/**< [in] Device address number. */
	uint32_t reg_addr, reg_bit;
	bool ret;

	switch (avago_get_sdrev(ah, addr)) {
	default:
	case AVAGO_SDREV_HVD6:
	case AVAGO_SDREV_16:
	case AVAGO_SDREV_D6:
		reg_addr = 0x24;
		reg_bit = 1 << 1;
		break;
	case AVAGO_SDREV_P1:
		reg_addr = 0xec;
		reg_bit = 1 << 1;
		break;
	case AVAGO_SDREV_OM4:
	case AVAGO_SDREV_CM4:
	case AVAGO_SDREV_CM4_16:
		reg_addr = 0xe8;
		reg_bit = 1 << 3;
		break;
	}
	ret = (avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_addr) & reg_bit) ?
	    true : false;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, rx_input_loopback=%s\n",
			 avago_addr_to_str(addr), ret ? "internal" : "external");

	return ret;
}

/** @brief   Sets the RX signal input source. */
/** @details Set <b>internal_loopback</b> true to select loopback from own TX output. */
/** @details Set to false to select external signal. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_rx_input_loopback(). */
int avago_serdes_set_rx_input_loopback(struct avago_hdl *ah,
					    /**< [in] Pointer to struct avago_hdl structure. */
				       uint32_t addr,
					/**< [in] Device address number. */
				       bool internal_loopback)
{
/**< [in] New loopback state. */
	int int_data = internal_loopback ? 0x0101 : 0x0100;

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, loopback=%s\n",
			 avago_addr_to_str(addr),
			 internal_loopback ? "internal" : "external");
	if (!avago_spico_int_check(ah, __func__, __LINE__, addr, 0x08, int_data))
		return -1;
	return 0;
}

int avago_serdes_get_int30_bits(struct avago_hdl *ah, uint32_t addr,
				uint32_t mask)
{
	int sdrev = avago_get_sdrev(ah, addr);
	int bits = 0;
	uint32_t pcs_read;

	if (mask & 0x0007) {
		int reg_spico_Clk_sel = (sdrev == AVAGO_SDREV_P1) ? 0xd6 : 0x0c;
		bits |= (avago_serdes_mem_rd(ah, addr, AVAGO_LSB,
					     reg_spico_Clk_sel) >> 8) & 0x7;
	}
	if (mask & 0x00f0) {
		int reg_pll_misc, reg_clk_misc, clk_shift;
		switch (sdrev) {
		default:
		case AVAGO_SDREV_OM4:
		case AVAGO_SDREV_CM4:
		case AVAGO_SDREV_CM4_16:
		case AVAGO_SDREV_16:
			reg_clk_misc = 0xf0;
			clk_shift = 3;
			reg_pll_misc = 0xd1;
			break;
		case AVAGO_SDREV_HVD6:
			reg_clk_misc = 0x300;
			clk_shift = 3;
			reg_pll_misc = 0x008;
			break;
		case AVAGO_SDREV_D6:
			reg_clk_misc = 0x300;
			clk_shift = 6;
			reg_pll_misc = 0x200;
			break;
		case AVAGO_SDREV_P1:
			break;
		}
		if (sdrev == AVAGO_SDREV_P1)
			bits |= AVAGO_SERDES_TX_PLL_REFCLK;
		else {
			bits |= (avago_serdes_mem_rd(ah, addr, AVAGO_ESB,
						     reg_clk_misc) >>
							clk_shift << 6) & 0xc0;
			bits |= (avago_serdes_mem_rd(ah, addr, AVAGO_ESB,
						     reg_pll_misc) >> 10 << 4) & 0x30;
		}
		if ((bits & 0x10) == 0x00)
			bits &= 0xff0f;
		else if ((bits & 0x30) == 0x10)
			bits &= 0xff1f;
		else if ((bits & 0x70) == 0x30)
			bits &= 0xff3f;
	}
	if (mask & 0x0f00) {
		int reg_rxdiv, pcsdiv_shift, mode66_bit;
		switch (sdrev) {
		default:
		case AVAGO_SDREV_OM4:
		case AVAGO_SDREV_CM4:
		case AVAGO_SDREV_CM4_16:
		case AVAGO_SDREV_P1:
			return bits;
		case AVAGO_SDREV_HVD6:
			return bits;
		case AVAGO_SDREV_16:
			reg_rxdiv = 0xc1;
			pcsdiv_shift = 8;
			mode66_bit = 1 << 3;
			break;
		case AVAGO_SDREV_D6:
			reg_rxdiv = 0x73;
			pcsdiv_shift = 2;
			mode66_bit = 1 << 0;
			break;
		}

		pcs_read = avago_serdes_mem_rd(ah, addr, AVAGO_ESB, reg_rxdiv);
		if ((pcs_read & mode66_bit) == 0)
			bits |= ((pcs_read >> pcsdiv_shift << 8) & 0x0700)
				| 0x0800;
	}
	return bits;
}

/** @brief   Gets the SerDes's SPICO clock source. */
/** @details */
/** @return  The SerDes's SPICO clock source. */
/** @return  On error, ah->return_code is set negative. */
/** @see     avago_serdes_set_spico_clk_src(), avago_serdes_get_tx_pll_clk_src(), avago_serdes_get_pcs_fifo_clk_div(). */
enum avago_serdes_spico_clk avago_serdes_get_spico_clk_src(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
							   uint32_t addr)
{
/**< [in] Device address number. */
	enum avago_serdes_spico_clk ret =
	    (enum avago_serdes_spico_clk)avago_serdes_get_int30_bits(ah, addr,
								     0x0007);
	return ret;
}

/** @brief   Sets the SerDes's SPICO clock source. */
/** @details */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_spico_clk_src(), avago_serdes_set_tx_pll_clk_src(), avago_serdes_set_pcs_fifo_clk_div(). */
int avago_serdes_set_spico_clk_src(struct avago_hdl *ah,
						/**< [in] Pointer to struct avago_hdl structure. */
				   uint32_t addr,
					    /**< [in] Device address number. */
				   enum avago_serdes_spico_clk src)
{
/**< [in] New clock source. */
	int return_code = ah->return_code;
	int mask;

	if (avago_get_ip_type(ah, addr) == AVAGO_P1)
		addr = avago_make_sbus_lane_addr(addr, AVAGO_ADDR_QUAD_ALL);
	mask = avago_serdes_get_int30_bits(ah, addr, ~7) | (src & 7);
	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x30, mask);

	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Gets the TX PLL clock source. */
/** @details */
/** @return  Returns the TX PLL clock source. */
/** @return  On error, decrements ah->return_code. */
/** @see     avago_serdes_set_tx_pll_clk_src(), avago_serdes_get_spico_clk_src(), avago_serdes_get_pcs_fifo_clk_div(). */
enum avago_serdes_tx_pll_clk avago_serdes_get_tx_pll_clk_src(struct avago_hdl
							     *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
							     uint32_t addr)
{
/**< [in] Device address number. */
	enum avago_serdes_tx_pll_clk ret =
	    (enum avago_serdes_tx_pll_clk)avago_serdes_get_int30_bits(ah, addr,
								      0x00f0);
	return ret;
}

/** @brief   Sets the SerDes's PLL Reference clock source. */
/** @details */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_tx_pll_clk_src(), avago_serdes_set_spico_clk_src(), avago_serdes_set_pcs_fifo_clk_div(). */
int avago_serdes_set_tx_pll_clk_src(struct avago_hdl *ah,
						/**< [in] Pointer to struct avago_hdl structure. */
				    uint32_t addr,
					    /**< [in] Device address number. */
				    enum avago_serdes_tx_pll_clk src)
{
/**< [in] New clock source. */
	int return_code = ah->return_code;
	int mask;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, tx_pll_clk=0x%x\n", avago_addr_to_str(addr),
			 src);

	mask = avago_serdes_get_int30_bits(ah, addr, ~0x00f0) | src;
	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x30, mask);
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Reads whether or not signal OK has been initialized. */
/** @return  Returns true if signal OK detection is enabled, false if not. */
/** @return  On error, decrements ah->return_code and returns false. */
/** @see     avago_serdes_initialize_signal_ok(). */
bool avago_serdes_get_signal_ok_enable(struct avago_hdl * ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				       uint32_t addr)
{
/**< [in] Device address number. */
	int sdrev = avago_get_sdrev(ah, addr);
	bool ret = false;
	if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false, 1,
				     0x1046)) {
		if (sdrev == AVAGO_SDREV_P1) {
			ret = !((avago_serdes_mem_rd(ah, addr, AVAGO_ESB, 0xa0)
				 >> 7) & 1);
			if (ret) {
				int val = avago_serdes_mem_rd(ah, addr,
							      AVAGO_ESB, 0xa1);
				ret = (val & 2) == 2;
			}
		} else if (sdrev != AVAGO_SDREV_D6 && sdrev != AVAGO_SDREV_HVD6) {
			int val = avago_serdes_mem_rd(ah, addr, AVAGO_ESB, 0xc4);
			ret = ((val >> 8) & 0x03) == 0x02;
		} else {
			int val = avago_serdes_mem_rd(ah, addr, AVAGO_ESB, 0x080);
			ret = (val & 0x03) == 0x02;
		}
	}
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, signal_ok_enable=%s\n",
			 avago_addr_to_str(addr), avago_bool_to_str(ret));

	return ret;
}

/** @brief   Checks for presence of electrical idle. */
/** @return  Returns true if electrical idle is detected, false if a signal is */
/**          detected. */
bool avago_serdes_get_electrical_idle(struct avago_hdl * ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				      uint32_t addr)
{
/**< [in] Device address number. */
	int reg_rx_elec_idle =
	    (avago_get_sdrev(ah, addr) == AVAGO_SDREV_P1) ? 0xed : 0x25;
	int val = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_rx_elec_idle);
	bool ret = (val & 0x04) == 0x04;

	return ret;
}

/** @brief   Reads the signal OK threshold value. */
/** @details Reads a sticky signal lost detector and returns whether any */
/**          loss was detected since the last call to this function. */
/**          Resets the detector if signal loss was detected so can tell */
/**          if a signal is once again present. */
/** @return  On success, returns threshold value. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_initialize_signal_ok(). */
int avago_serdes_get_signal_ok_threshold(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
					 uint32_t addr)
{
/**< [in] Device address number. */
	int return_code = ah->return_code;
	int threshold = 0;
	if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false, 1,
				     0x1046)) {
		int reg, shift, mask;
		switch (avago_get_sdrev(ah, addr)) {
		default:
		case AVAGO_SDREV_HVD6:
		case AVAGO_SDREV_D6:
			reg = 0x80;
			shift = 2;
			mask = 0x0f;
			break;
		case AVAGO_SDREV_16:
			reg = 0xc4;
			shift = 0;
			mask = 0x1f;
			break;
		case AVAGO_SDREV_P1:
			reg = 0xa1;
			shift = 2;
			mask = 0x0f;
			break;
		case AVAGO_SDREV_CM4:
			reg = 0xff;
			shift = 2;
			mask = 0x0f;
			break;
		case AVAGO_SDREV_OM4:
			reg = 0;
			return 0;
		}
		threshold = avago_serdes_mem_rd(ah, addr, AVAGO_ESB, reg);
		threshold = (threshold >> shift) & mask;
	}
	if (return_code != ah->return_code)
		threshold = -1;

	return threshold;
}

/** @brief   Reads and resets the signal_ok_deasserted signal. */
/** @details Reads a sticky signal lost detector and returns whether any */
/**          loss was detected since the last call to this function. */
/**          Resets the detector if signal loss was detected so can tell */
/**          if a signal is once again present. */
/** @return  Returns true if no signal loss has been detected. */
/** @return  Returns false if signal loss was detected since last check. */
/** @see     avago_serdes_initialize_signal_ok(). */
bool avago_serdes_get_signal_ok(struct avago_hdl * ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				uint32_t addr,
				/**< [in] Device address number. */
				bool reset)
{
/**< [in] Clear signal loss sticky bit if set. */
	int reg_serdes_rdy =
	    (avago_get_sdrev(ah, addr) == AVAGO_SDREV_P1) ? 0xee : 0x26;
	int bits = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_serdes_rdy);
	bool sig_ok = (bits & 0x0010) ? 0 : 1;
	if (reset && !sig_ok)
		avago_serdes_mem_rmw(ah, addr, AVAGO_LSB, reg_serdes_rdy, 0,
				     0x0010);

	return sig_ok;
}

/** @brief   Reads the live version of signal_ok */
/** @details Reads signal_ok from O_CORE_STATUS port */
/** @return  Returns true if signal OK is true */
/** @return  Returns false if signal OK is false */
/** @see     avago_serdes_initialize_signal_ok(). */
bool avago_serdes_get_signal_ok_live(struct avago_hdl * ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				     uint32_t addr)
{
/**< [in] Device address number. */
	int reg_serdes_rdy =
	    (avago_get_sdrev(ah, addr) == AVAGO_SDREV_P1) ? 0xee : 0x26;
	int bits = avago_serdes_mem_rd(ah, addr, AVAGO_LSB, reg_serdes_rdy + 1);
	bool sig_ok = (bits & 0x0010) ? 1 : 0;

	return sig_ok;
}

/** @brief   Enables the RX Idle Detector to aide detecting signal loss. */
/** @details Call this function once to initialize the idle detector. */
/**          Then call avago_serdes_get_signal_ok() to check if signal loss is detected. */
/**          Functional for firmware 0x1046 and newer.  No-op on older versions. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_serdes_get_signal_ok(), avago_serdes_get_signal_ok_threshold(). */
int avago_serdes_initialize_signal_ok(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				      uint32_t addr,
				/**< [in] Device address number. */
				      int threshold)
{
/**< [in] Threshold for signal OK (0-15). */
	int return_code = ah->return_code;
	int ip_rev = avago_get_ip_rev(ah, addr);
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, threshold %d\n", avago_addr_to_str(addr),
			 threshold);

	if (ip_rev <= 0x0c && ip_rev >= 0x0a &&
	    avago_get_process_id(ah, addr) == AVAGO_PROCESS_B) ;
	else if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false,
					  1, 0x1046)) {
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x20, 0x20);
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x20,
				      0x40 | ((threshold & 0xf) << 8));
	}
	avago_serdes_get_signal_ok(ah, addr, true);
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Returns whether or not the RX PLL is frequency locked. */
/** @details NOTE: The PLL may be locked even when valid data is not */
/**          present at the RX inputs. The user should generally rely */
/**          on signal ok instead. */
/** @return  Returns true if the PLL is locked */
/** @return  Returns false if the PLL is not locked */
bool avago_serdes_get_frequency_lock(struct avago_hdl * ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
				     uint32_t addr)
{
/**< [in] Device address number. */
	bool flock =
	    (avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x1c) & 0x8000) >> 15;
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, freq lock=%d\n", avago_addr_to_str(addr),
			 flock);

	return flock;
}

/** @brief   Retrieves the value of the error counter. */
/** @details If reset is selected, resets the error counter after reading. */
/** @return  On success, returns the error count. */
/** @return  On error, decrements ah->return_code and returns an all ones value. */
/** @see     avago_serdes_error_reset(). */
uint32_t avago_serdes_get_errors(struct avago_hdl * ah,
					      /**< [in] Pointer to struct avago_hdl structure. */
				 uint32_t addr,
					  /**< [in] Device address number. */
				 enum avago_serdes_mem_type type,
				      /**< [in] Unused, set to AVAGO_LSB. */
				 bool reset)
{
/**< [in] Set to reset error count after read. */
	int return_code = ah->return_code;
	uint32_t rc, err_count;
	(void)type;

	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x18, 3);
	err_count = avago_spico_int(ah, addr, 0x1a, 0);
	err_count += avago_spico_int(ah, addr, 0x1a, 0) << 16;

	if (reset)
		avago_serdes_error_reset(ah, addr);

	rc = return_code == ah->return_code ? err_count : ~0U;

	return rc;
}

/** @brief   Retrieves the value of the simple error flag.
 ** @details If reset is selected, resets the error flag after reading.
 ** @return  On success, returns the whether or not the error flag has been set.
 ** @return  On error, decrements ah->return_code and returns an all ones value.
 ** @see     avago_serdes_error_flag_reset().
 **/
bool avago_serdes_get_error_flag(struct avago_hdl *ah, uint32_t addr, bool reset)
{
	bool error_flag = (avago_serdes_mem_rd(ah, addr, AVAGO_LSB, 0x18) & 0x40) >> 6;
	if (reset)
		avago_serdes_error_flag_reset(ah, addr);

	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, reset=%d, errors = %u.\n",
			 avago_addr_to_str(addr), reset, error_flag);
	return error_flag;
}

/** @brief   Resets the error flag.
 ** @see     avago_serdes_get_errors().
 ** @return  On success, returns TRUE.
 ** @return  On failure, returns FALSE and decrements ah->return_code.
 **/
void avago_serdes_error_flag_reset(struct avago_hdl *ah, uint32_t addr)
{
	avago_serdes_mem_rmw(ah, addr, AVAGO_LSB, 0x18, 0x10, 0x10);
	avago_serdes_mem_rmw(ah, addr, AVAGO_LSB, 0x18, 0x00, 0x10);
	AVAGO_LOG_PRINT5(ah, AVAGO_DEBUG5, __func__, __LINE__, "SBus %s\n",
			 avago_addr_to_str(addr));
}

/** @brief   Resets the error counter. */
/** @see     avago_serdes_get_errors(). */
/** @return  On success, returns true. */
/** @return  On failure, returns false and decrements ah->return_code. */
bool avago_serdes_error_reset(struct avago_hdl * ah,
				/**< [in] Pointer to struct avago_hdl structure. */
			      uint32_t addr)
{
/**< [in] Device address number. */
	bool rc = avago_spico_int_check(ah, __func__, __LINE__, addr, 0x17, 0);

	return rc;
}

/** @brief   Configures core_to_cntl to enable tx/rx enable and/or 802.3AZ controls. */
/** @details Enables i_core_to_cntl control of certain functions. */
/** @return  On success, returns true. */
/** @return  On failure, returns false and decrements ah->return_code. */
bool avago_serdes_enable_core_to_cntl(struct avago_hdl * ah,
					/**< [in] Pointer to struct avago_hdl structure. */
				      uint32_t addr,
				    /**< [in] Device address number. */
				      bool enable_tx_rx_en,
				/**< [in] Enable i_core_to_cntl of Tx & Rx enable. */
				      bool enable_low_power)
{
/**< [in] Enable i_core_to_cntl of Tx & Rx quiet, and lpi_disable. */
	bool rc;
	int data = 0;
	if (enable_tx_rx_en)
		data |= 1;
	if (enable_low_power)
		data |= 2;
	rc = avago_spico_int_check(ah, __func__, __LINE__, addr, 0x27, data);

	return rc;
}

/** @brief   Slips (drops) bits on the Rx. */
/** @details Slips up to 127 bits. */
/** @return  On success, returns true. */
/** @return  On failure, returns false and decrements ah->return_code. */
bool avago_serdes_slip_bits(struct avago_hdl * ah,
				/**< [in] Pointer to struct avago_hdl structure. */
			    uint32_t addr,
			    /**< [in] Device address number. */
			    uint32_t bits)
{
/**< [in] Number of bits to slip.  127 max. */
	return avago_spico_int_check(ah, __func__, __LINE__, addr, 0x0c,
				     bits & 0x7f);
}

/** @brief   Slips the Rx FIFO clock phase. */
/** @details Valid values are 1 to the word width. */
/** @return  On success, returns true. */
/** @return  On failure, returns false and decrements ah->return_code. */
bool avago_serdes_slip_rx_phase(struct avago_hdl * ah,
				/**< [in] Pointer to struct avago_hdl structure. */
				uint32_t addr,
			    /**< [in] Device address number. */
				uint32_t bits,
			    /**< [in] Number of bits to slip. 63 max. */
				bool remember)
{
/**< [in] If true, applies slips every time the Rx is enabled. */
	int flags = remember ? 0x8000 : 0;
	return avago_spico_int_check(ah, __func__, __LINE__, addr, 0x0e,
				     ((bits & 0x3f) << 8) | flags);
}

/** @brief   Slips the Tx phase. */
/** @details Each slip slips the Tx load by 2 UI.  Up to 31 slips can be done. */
/** @return  On success, returns true. */
/** @return  On failure, returns false and decrements ah->return_code. */
bool avago_serdes_slip_tx_phase(struct avago_hdl * ah,
				/**< [in] Pointer to struct avago_hdl structure. */
				uint32_t addr,
			    /**< [in] Device address number. */
				uint32_t slips,
			    /**< [in] Number of slips (2UI each) to perform. */
				bool remember)
{
/**< [in] If true, applies slips every time the Tx is enabled. */
	int flags = remember ? 0x8000 : 0;
	return avago_spico_int_check(ah, __func__, __LINE__, addr, 0x0d,
				     (slips & 0x1f) | flags);
}

/** @brief   Allocates and initializes a struct avago_serdes_init_config struct. */
/** @details The return value should be released using */
/**          avago_serdes_init_config_destruct() after use. */
/** @return  On success, returns a pointer to the initialized structure. */
/** @return  On failure, returns NULL. */
/** @see     avago_serdes_init_config_destruct(), avago_serdes_init(). */
struct avago_serdes_init_config
*avago_serdes_init_config_construct(struct avago_hdl *ah)
{
/**< [in] Pointer to struct avago_hdl structure. */
	size_t bytes = sizeof(struct avago_serdes_init_config);
	struct avago_serdes_init_config *config;

	if (!(config = (struct avago_serdes_init_config *)malloc(bytes)))
		return NULL;
	memset(config, 0, sizeof(*config));

	config->sbus_reset = true;
	config->spico_reset = true;
	config->init_mode = AVAGO_PRBS31_ILB;

	config->init_tx = true;
	config->tx_divider = 10;
	config->tx_width = 40;
	config->tx_phase_cal = false;
	config->refclk_sync_master = true;
	config->tx_output_en = true;

	config->init_rx = true;
	config->rx_divider = 10;
	config->rx_width = 40;
	config->signal_ok_en = true;
	config->signal_ok_threshold = 0x0;

	return config;
}

/** @brief   Releases a struct avago_serdes_init_config struct. */
/** */
/** @return  None. */
/** @see     avago_serdes_init_config_construct(), avago_serdes_init(). */
void avago_serdes_init_config_destruct(struct avago_hdl *ah,
						    /**< [in] Pointer to struct avago_hdl structure. */
				       struct avago_serdes_init_config *config)
{
/**< [in] Pointer to struct to release. */
	free(config);
}

/** @brief   Initializes a SerDes device to the given divider. */
/** */
/** @return  0 on success, */
/** @return  > 0 if channel errors after initialization, */
/** @return  ah->return_code (< 0) if errors during initialization. */
/** @see     avago_serdes_init(). */
int avago_serdes_init_quick(struct avago_hdl *ah,
				/**< [in] Pointer to struct avago_hdl structure. */
			    uint32_t addr,
			    /**< [in] Device address number. */
			    uint32_t divider)
{
/**< [in] Desired divider to configure. */
	int errors;
	struct avago_serdes_init_config *config =
	    avago_serdes_init_config_construct(ah);
	config->tx_divider = divider;
	config->rx_divider = divider;
	errors = avago_serdes_init(ah, addr, config);
	avago_serdes_init_config_destruct(ah, config);
	return errors;
}

/** @brief Provides a valid, device-specific width mode.
 * *        Generally, wider widths are preferred.
 **/
static uint32_t avago_serdes_get_default_width(struct avago_hdl *ah,
					       uint32_t addr)
{
	uint32_t width = 40;
	if (avago_get_ip_type(ah, addr) == AVAGO_M4)
		width = 80;
	return width;
}

/** @brief   Configures and calibrates a SerDes. */
/** @details Call this to initialize a SerDes. */
/** @return  0 on success, ah->return_code (< 0) on error. */
/** @see     avago_serdes_init_config_construct(), avago_serdes_init_config_destruct(), avago_serdes_init_quick(). */
int avago_serdes_init(struct avago_hdl *ah,	    /**< [in] Pointer to struct avago_hdl structure. */
		      uint32_t addr,		/**< [in] Device address number. */
		      struct avago_serdes_init_config *config)
{
/**< [in] Desired SerDes configuration. */
	bool tx_en = false, rx_en = false, tx_output_en = false;
	uint32_t errors;
	int sdrev = avago_get_sdrev(ah, addr);
	if (avago_check_broadcast_address(ah, addr, __func__, __LINE__, true))
		return -1;
	if (!avago_check_ip_type(ah, addr, __func__, __LINE__, true, 3,
				 AVAGO_SERDES, AVAGO_M4, AVAGO_P1) ||
	    !avago_check_process(ah, addr, __func__, __LINE__, true, 2,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F))
		return -1;

	avago_log_printf(ah, AVAGO_DEBUG2, __func__, __LINE__,
			 "SBus %s\n", avago_addr_to_str(addr));
	if (config->sbus_reset)
		avago_sbus_reset(ah, addr, 0);
	if (config->spico_reset)
		avago_spico_reset(ah, addr);

	if (sdrev == AVAGO_SDREV_P1) {
		struct avago_addr addr_struct;
		avago_addr_to_struct(addr, &addr_struct);
		if (addr_struct.lane < 8)
			return avago_fail(ah, __func__, __LINE__,
					  "SBus %s: SerDes P1 requires quad or chip level initialization.\n",
					  avago_addr_to_str(addr));

		if (config->tx_divider != config->rx_divider)
			return avago_fail(ah, __func__, __LINE__,
					  "SBus %s: SerDes P1 requires Tx and Rx dividers to match.\n",
					  avago_addr_to_str(addr));

		if (addr_struct.lane == AVAGO_ADDR_QUAD_ALL &&
		    !avago_is_octal_p1(ah, addr))
			addr = avago_make_sbus_lane_addr(addr,
							 AVAGO_ADDR_QUAD_LOW);
	}

	avago_set_spico_running_flag(ah, addr, 1);

	if (!avago_spico_crc(ah, addr)) {
		return avago_fail(ah, __func__, __LINE__,
				  "SerDes %s failed CRC check.\n",
				  avago_addr_to_str(addr));
		return -1;
	}

	if (!config->sbus_reset) {
		avago_serdes_get_tx_rx_ready(ah, addr, &tx_en, &rx_en);
		tx_output_en = avago_serdes_get_tx_output_enable(ah, addr);
	}

	avago_serdes_set_tx_rx_enable(ah, addr, false, false, false);
	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x11, 0x00);

	avago_spico_int_check(ah, __func__, __LINE__, addr, 0x0b,
			      config->tx_phase_cal ? 1 : 0);

	if (config->tx_divider == config->rx_divider) {
		int div_val = (config->tx_divider & 0xff) | (1 << 15);
		if (config->refclk_sync_master)
			div_val |= 1 << 12;
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x05,
				      div_val);
	} else {
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x05,
				      (config->tx_divider & 0xff) |
				      (config->refclk_sync_master ?
					1 << 12 : 0));
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x06,
				      config->rx_divider & 0xff);
	}

	if (config->tx_width == 0)
		config->tx_width = avago_serdes_get_default_width(ah, addr);
	if (config->rx_width == 0)
		config->rx_width = avago_serdes_get_default_width(ah, addr);
	avago_serdes_set_tx_rx_width_pam(ah, addr, config->tx_width,
					 config->rx_width, config->tx_encoding,
					 config->rx_encoding);

	avago_serdes_set_tx_rx_enable(ah, addr, config->init_tx,
				      config->init_rx,
				      config->tx_output_en | tx_output_en);

	if (config->signal_ok_en && sdrev != AVAGO_SDREV_P1)
		avago_serdes_initialize_signal_ok(ah, addr,
						  config->signal_ok_threshold);

	if (config->init_mode == AVAGO_INIT_ONLY) {
		avago_serdes_set_rx_input_loopback(ah, addr, false);
		avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__, "\n");
		return 0;
	}
	avago_serdes_set_rx_input_loopback(ah, addr, true);
	if (config->init_tx)
		avago_serdes_set_tx_data_sel(ah, addr,
					     AVAGO_SERDES_TX_DATA_SEL_PRBS31);
	if (config->init_rx) {
		if (config->init_tx && sdrev == AVAGO_SDREV_OM4 &&
		    avago_get_jtag_idcode(ah, 0) == 0x0973957f)
			avago_serdes_set_tx_invert(ah, addr, true);
		if (config->init_tx &&
		    config->rx_encoding == AVAGO_SERDES_PAM4 &&
		    (sdrev == AVAGO_SDREV_CM4 || sdrev == AVAGO_SDREV_CM4_16 ||
		     sdrev == AVAGO_SDREV_OM4)) {
			avago_spico_int_check(ah, __func__, __LINE__, addr,
					      0x0a, 1);
			avago_serdes_dfe_wait(ah, addr);
		}
		avago_serdes_set_rx_cmp_data(ah, addr,
					     AVAGO_SERDES_RX_CMP_DATA_PRBS31);
		avago_spico_int_check(ah, __func__, __LINE__, addr, 0x03,
				AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN | 0x0003);
	}

	if (sdrev == AVAGO_SDREV_HVD6) {
		avago_serdes_mem_rmw(ah, addr, AVAGO_LSB, 0x21, 0x80, 0x80);
		avago_serdes_mem_wr(ah, addr, AVAGO_LSB, 0x1f, 0x1604);
		avago_serdes_mem_rmw(ah, addr, AVAGO_ESB, 0, 0, 0x800);

		avago_serdes_set_tx_data_sel(ah, addr,
					     AVAGO_SERDES_TX_DATA_SEL_PRBS31);
		avago_serdes_set_rx_cmp_data(ah, addr,
					     AVAGO_SERDES_RX_CMP_DATA_PRBS31);
		if (config->init_tx && config->init_rx)
			errors = avago_serdes_error_reset(ah, addr);
	}

	if (config->init_tx && config->init_rx)
		errors = avago_serdes_get_errors(ah, addr, AVAGO_LSB, 1);
	else
		errors = 0;

	if (config->init_mode == AVAGO_PRBS31_ELB ||
	    config->init_mode == AVAGO_CORE_DATA_ELB) {
		if (config->tx_inversion)
			avago_serdes_set_tx_invert(ah, addr, true);
		if (config->rx_inversion)
			avago_serdes_set_rx_invert(ah, addr, true);
		avago_serdes_set_rx_input_loopback(ah, addr, false);
	}
	if (config->init_mode == AVAGO_CORE_DATA_ILB ||
	    config->init_mode == AVAGO_CORE_DATA_ELB) {
		avago_serdes_set_tx_data_sel(ah, addr,
					     AVAGO_SERDES_TX_DATA_SEL_CORE);
		avago_serdes_set_rx_cmp_data(ah, addr,
					     AVAGO_SERDES_RX_CMP_DATA_OFF);
	}

	avago_log_printf(ah, AVAGO_DEBUG1, "", 0, "Errors in ILB: %u.\n", errors);

	if (errors) {
		avago_log_printf(ah, AVAGO_WARNING, __func__, __LINE__,
				 "%u PRBS errors detected during ILB check of SerDes %s during serdes_init to div %d. Use this SerDes with caution.\n",
				 errors, avago_addr_to_str(addr),
				 config->tx_divider);
	}

	return (int)errors;
}

/**
 * @brief Initialize struct with default values
 *
 * @param	ah	Avago handle
 * @param[out]	control	tune options structure to initialize.
 */

void avago_serdes_tune_init(struct avago_hdl *ah,
			    struct avago_serdes_dfe_tune *control)
{
	(void)ah;
	memset(control, 0, sizeof(*control));

	control->tune_mode = AVAGO_DFE_ICAL;

	control->value[0] = 56;
	control->value[1] = 12;
	control->value[2] = 0;
	control->value[3] = 15;
}

/** @brief  Starts SerDes tuning with a few options. */
/** @note   Requires SerDes firmware revision 0x1046 or higher. */
bool avago_serdes_tune(struct avago_hdl *ah,	    /**< [in] Pointer to struct avago_hdl structure. */
		       uint32_t addr,		/**< [in] Device address number. */
		       struct avago_serdes_dfe_tune *control)
{
/**< [in] Tune options. */
	uint32_t int_data = 0;
	uint32_t int_data2 = 0;
	uint32_t column = (AVAGO_SDREV_P1 == avago_get_sdrev(ah, addr)) ? 0 : 2;

	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__, "SBus %s, tune_mode %s\n",
			 avago_addr_to_str(addr),
			 avago_dfe_tune_mode_to_str(control->tune_mode));
	if (!avago_check_ip_type(ah, addr, __func__, __LINE__, true, 2,
				 AVAGO_SERDES, AVAGO_M4))
		return false;
	if (!avago_check_process(ah, addr, __func__, __LINE__, true, 2,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F))
		return false;
	if (!avago_check_firmware_rev(ah, addr, __func__, __LINE__, true, 1,
				      0x1046))
		return false;

	if (control->dfe_disable) {
		int_data |= 0x0040;
	}

	if (control->fixed[0]) {
		int_data |= 0x0080;
		avago_spico_int(ah, addr, 0x26,
				(column << 12) | (2 << 8) |
				(control->value[0] & 0xff));
	}
	if (control->fixed[1]) {
		int_data |= 0x0100;
		avago_spico_int(ah, addr, 0x26,
				(column << 12) | (1 << 8) |
				(control->value[1] & 0xff));
	}
	if (control->fixed[2]) {
		int_data |= 0x0200;
		avago_spico_int(ah, addr, 0x26,
				(column << 12) | (0 << 8) |
				(control->value[2] & 0xff));
	}

	if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false,
				     1, 0x104D)
	    && control->fixed[3]) {
		int_data |= 0x0400;
		avago_spico_int(ah, addr, 0x26,
				(2 << 12) | (3 << 8) |
				(control->value[3] & 0xff));
	}

	switch (control->tune_mode) {
	case AVAGO_DFE_ICAL:
			int_data |= 0x0001;
			int_data2 = 0x5b00;
			break;

	case AVAGO_DFE_PCAL:
			int_data |= 0x0002;
			break;

	case AVAGO_DFE_ICAL_ONLY:
			int_data |= 0x0001;
			int_data2 = 0x5b01;
			break;

	case AVAGO_DFE_ENABLE_RR:
			int_data |= 0x000A;
			break;

	case AVAGO_DFE_START_ADAPTIVE:
			int_data |= 0x0006;
			break;

	case AVAGO_DFE_STOP_ADAPTIVE:
			int_data = 0x0002;
			break;

	case AVAGO_DFE_DISABLE_RR:
			int_data = 0x0008;
			break;
	}

	if (int_data2)
		avago_spico_int(ah, addr, 0x26, int_data2);
	return avago_spico_int_check(ah, __func__, __LINE__,
				     addr, 0x0a, int_data);
}
