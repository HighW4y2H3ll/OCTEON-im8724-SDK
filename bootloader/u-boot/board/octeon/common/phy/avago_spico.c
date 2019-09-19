/* AAPL CORE Revision: 2.3.0 */

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
 * * @file spico.c
 ** @brief Functions for SPICO processors.
 **/

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_serdes.h"
#include "avago_spico.h"
#include "avago_sbus.h"
#include "avago_logging.h"

/**
 * Check the PC and possible interrupt pending bits to see if the processor is
 * running or is hung.
 *
 * @param[in]	ah		Avago handle
 * @param	sbus_addr	SBus address of SerDes
 *
 * @return	0 if SPICO is running properly for the slice, -1 if hung.
 */
int avago_spico_running(struct avago_hdl *ah, uint32_t sbus_addr)
{
	int running = 0;

	if (!avago_check_process(ah, sbus_addr, __func__, __LINE__, true, 3,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F, AVAGO_PROCESS_A) ||
	    avago_check_broadcast_address(ah, sbus_addr, __func__, __LINE__,
					  true))
		return 0;

	switch (avago_get_process_id(ah, sbus_addr)) {
	case AVAGO_PROCESS_B:
	case AVAGO_PROCESS_F:
		if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					 true, 4, AVAGO_SERDES,
					 AVAGO_SPICO, AVAGO_M4, AVAGO_P1))
			return 0;
		if ((avago_get_ip_type(ah, sbus_addr) == AVAGO_SERDES) ||
		    (avago_get_ip_type(ah, sbus_addr) == AVAGO_M4) ||
		    (avago_get_ip_type(ah, sbus_addr) == AVAGO_P1)) {
			int pc = avago_sbus_rd(ah, sbus_addr, 0x25);
			int interrupt = avago_sbus_rd(ah, sbus_addr, 0x4);
			int mem_bist = avago_sbus_rd(ah, sbus_addr, 0x9);
			int stepping = avago_sbus_rd(ah, sbus_addr, 0x20);
			int enable = avago_sbus_rd(ah, sbus_addr, 0x7);
			int error = avago_sbus_rd(ah, sbus_addr, 0x2a);

			if (((enable & 0x2) == 0x2) && (pc != 0x2) &&
			    (pc != 0xffff) &&
			    ((interrupt >> 16 & 0x3) == 0x0) &&
			    ((mem_bist & 0x1) == 0x00) &&
			    ((stepping & 1) == 0) &&
			    ((error & 0x1f) != 0x1f))
				running = 1;
		} else {
			int pc = avago_sbus_rd(ah, sbus_addr, 0xa);
			int stepping = avago_sbus_rd(ah, sbus_addr, 0x05);
			int mem_bist = avago_sbus_rd(ah, sbus_addr, 0x0);
			int enable = avago_sbus_rd(ah, sbus_addr, 0x1);
			int error = avago_sbus_rd(ah, sbus_addr, 0xf);

			if ((pc != 0x2) && (pc != 0xffff) &&
			    ((stepping & 1) == 0) &&
			    ((mem_bist & 0x4) == 0x00) &&
			    ((enable & 0x100) == 0x100) &&
			    ((error & 0x1f) != 0x1f))
				running = 1;
		}
		break;

	case AVAGO_PROCESS_A:
		if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, true, 1, AVAGO_SPICO))
			return 0;
		if (avago_sbus_rd(ah, sbus_addr, 0xa) != 0x2)
			running = 1;
		break;

	default:
		avago_fail(ah, __func__, __LINE__,
			  "SBus %s, IP type 0x%x, in process %s, is not supported.\n",
			  avago_addr_to_str(sbus_addr), avago_get_ip_type(ah,
									sbus_addr),
			  avago_get_process_id_str(ah, sbus_addr));
		break;

	}
	return running;
}

/** @brief   Internal function. Returns the value of SPICO's program counter. */
/** @return  The PC value */
/** @see     avago_spico_status */
uint32_t avago_spico_get_pc(struct avago_hdl *ah, uint32_t addr)
{
	switch (avago_get_ip_type(ah, addr)) {
	case AVAGO_P1:
	case AVAGO_M4:
	case AVAGO_SERDES:
		return avago_sbus_rd(ah, addr, 0x25);
	case AVAGO_SPICO:
		return avago_sbus_rd(ah, addr, 0x0a) |
			((avago_sbus_rd(ah, addr, 0x0e) & 0x3FF) << 16);
	default:
		return 0;
	}
}

/** @brief   Internal function. Returns the current state of the processor. */
/** @return  SPICO's state byte */
/** @see     avago_spico_status */
uint32_t avago_spico_get_state(struct avago_hdl *ah, uint32_t addr)
{
	switch (avago_get_ip_type(ah, addr)) {
	case AVAGO_P1:
	case AVAGO_M4:
	case AVAGO_SERDES:
		return avago_sbus_rd(ah, addr, 0x2a);
	case AVAGO_SPICO:
		return avago_sbus_rd(ah, addr, 0x0f) & 0x1f;
	default:
		return 0;
	}
}

/** */
/** @brief    Gets the current state of the processor */
/** @return   ah->return_code */
/** @details  Create a spico_status struct, if spico is not running or it's an invalid */
/** IP or Process the struct is returned with a default configuration (state = SPICO_RESET */
/** and PC=2, all other members = 0).  Otherwise the struct avago_spico_status is updated with the */
/** current information for the processor. */
int avago_spico_status(struct avago_hdl *ah,	/**< [in] Pointer to AAPL structure */
		       uint32_t sbus_addr,	/**< [in] SBus address of SerDes */
		       bool no_cache,	/**< [in] Don't use AAPL cache for returning firmware rev and build information */
		       struct avago_spico_status * state)
{
/**< [out] Pointer to structure to fill in */
	if (avago_get_spico_running_flag(ah, sbus_addr)) {
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, false, 1,
		     AVAGO_SPICO))
			state->clk = AVAGO_SERDES_SPICO_REFCLK;
		else
			state->clk =
			    avago_serdes_get_spico_clk_src(ah, sbus_addr);

		if (no_cache) {
			struct avago_addr addr_struct;
			avago_addr_to_struct(sbus_addr, &addr_struct);
			ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0;
			ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0;
		}
		state->revision = avago_get_firmware_rev(ah, sbus_addr);
		state->build = avago_get_firmware_build(ah, sbus_addr);
		state->pc = avago_spico_get_pc(ah, sbus_addr);
		state->enabled = 1;
		switch (avago_spico_get_state(ah, sbus_addr)) {
		case 0x00:
			state->state = AVAGO_SPICO_RESET;
			break;

		case 0x12:
			state->state = AVAGO_SPICO_PAUSE;
			break;

		case 0x1f:
			state->state = AVAGO_SPICO_ERROR;
			break;

		default:
			state->state = AVAGO_SPICO_RUNNING;
			break;
		}
	} else {

		state->enabled = 0;
		state->pc = 2;
		state->revision = 0;
		state->build = 0;
		state->state = AVAGO_SPICO_RESET;
		state->clk = AVAGO_SERDES_SPICO_REFCLK;
	}
	return ah->return_code;
}

static int add_int_slice_sel(struct avago_hdl *ah, uint32_t addr, int int_num)
{
	char tab[16] =
	    { 0, 0x08, 0x10, 0x18, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7 };
	int slice = tab[(addr >> 16) & 0xf];
	if (slice == 0 || avago_get_ip_type(ah, addr) != AVAGO_P1)
		return 0;
	if (int_num & 0xc000) {
		if (slice == 0x10)
			slice = 4;
		return (slice & 7) << 10;
	}

	return slice << 8;
}

/** @cond INTERNAL */

uint32_t avago_serdes_spico_int_le_fn(struct avago_hdl *ah,
			/**< [in] Pointer to AAPL structure */
				  uint32_t sbus_addr,
			/**< [in] SBus address of SBus Master SPICO */
				  int int_num,
			/**< [in] Interrupt code */
				  int param)
{
/**< [in] Interrupt data */
	uint32_t le_addr;
	int loops = 0;
	uint32_t data = 0;
	for (le_addr = sbus_addr;
	     le_addr <= avago_get_max_sbus_addr(ah, le_addr); le_addr++) {
		if (avago_get_ip_type(ah, le_addr) == AVAGO_LINK_EMULATOR)
			break;
	}
	avago_sbus_wr(ah, le_addr, 0x1, int_num | (param << 16));
	avago_sbus_wr(ah, le_addr, 0x2, 1);
	avago_sbus_wr(ah, le_addr, 0x2, 0);
	for (loops = 0; loops < ah->serdes_int_timeout; loops++) {
		data = avago_sbus_rd(ah, le_addr, 0x07);
		if ((data & 0x10000) == 0)
			break;
	}
	if (loops >= ah->serdes_int_timeout) {
		avago_set_spico_running_flag(ah, sbus_addr, 0);
		avago_fail(ah, __func__, __LINE__,
			  "Interrupt 0x%02x,0x%04x timed out after %d loops on SBus address %s -> 0x%x.\n",
			  int_num, param, ah->serdes_int_timeout,
			  avago_addr_to_str(sbus_addr), data);
		return 0;
	}
	return data;
}

/**
 * Function that implements SerDes SPICO interrupts via the SBus, which is the
 * default method
 *
 * @param[in]	ah		Avago handle
 * @param	sbus_addr	SBus address of SBus Master SPICO
 * @param	int_num		Interrupt code
 * @param	param		Interrupt data
 *
 * @return	Return data from interrupt
 */
uint32_t avago_serdes_spico_int_sbus_fn(struct avago_hdl *ah,
				    uint32_t sbus_addr,
				    int int_num,
				    int param)
{
	bool st;
	struct avago_addr addr_struct, start, stop, next;
	uint32_t dev_count = 0;
	uint32_t prev_data = 0;
	int loops = 0;
	uint32_t data = 0;

	avago_sbus_wr(ah, sbus_addr, 0x03, (int_num << 16) | param);
	avago_addr_to_struct(sbus_addr, &addr_struct);

	for (st =
	     avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
				  AVAGO_BROADCAST_IGNORE_LANE); st;
	     st =
	     avago_broadcast_next(ah, &next, &start, &stop,
				 AVAGO_BROADCAST_IGNORE_LANE)) {
		uint32_t sbus_addr = avago_struct_to_addr(&next);
		if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					 true, 3, AVAGO_SERDES,
					 AVAGO_M4, AVAGO_P1))
			continue;

		for (loops = 0; loops < ah->serdes_int_timeout; loops++) {
			data = avago_sbus_rd(ah, sbus_addr, 0x04);
			debug("%s: loop %d, data: 0x%x\n", __func__, loops, data);
			if ((data & 0x30000) == 0)
				break;
			if (avago_get_spico_running_flag(ah, sbus_addr) ==
			    AVAGO_SPICO_HALT) {
				avago_log_printf(ah, AVAGO_DEBUG1, __func__,
						__LINE__,
						"SBus %s in halted state, so return value may not be valid for interrupt 0x%02x,0x%04x.\n",
						avago_addr_to_str(sbus_addr),
						int_num, param);
				break;
			}
		}
		if (dev_count > 0 && prev_data != data) {
			printf("ERROR: (%s:%d): Broadcast interrupt failed because not all SerDes responded with the same answer. Previous answers: %04x. Current answer from SBus address %x: %04x.\n",
			       __func__, __LINE__, prev_data, sbus_addr, data);
			return 0;
		}
		prev_data = data;
		dev_count++;

		if (loops >= ah->serdes_int_timeout) {
			uint32_t bits = (data >> 16) & 3;
			const char *msg = bits == 3 ? "disabled & in_progress"
				: bits == 2 ? "disabled" : "in_progress";

			avago_set_spico_running_flag(ah, sbus_addr, false);
			printf("ERROR: (%s:%d): Interrupt 0x%02x,0x%04x timed out after %d loops on SBus address %x -> 0x%x (%s).\n",
			       __func__, __LINE__, int_num, param,
			       ah->serdes_int_timeout, sbus_addr, data, msg);
			return data;
		}
	}

	if (avago_get_lsb_rev(ah, sbus_addr) <= 2)
		data = avago_sbus_rd(ah, sbus_addr, 0x04);
	return data;
}

/** @endcond */

static uint32_t avago_serdes_spico_int(struct avago_hdl *ah,
			/**< [in] Pointer to AAPL structure */
				   uint32_t sbus_addr,
			/**< [in] SBus address of SBus Master SPICO */
				   int int_num,
			/**< [in] Interrupt code */
				   int param)
{
/**< [in] Interrupt data */
	int loops = 0;
	uint32_t data = 0;

	debug("%s(0x%p, %s, 0x%x, 0x%x)\n", __func__, ah,
	      avago_addr_to_str(sbus_addr),
	      int_num, param);
	if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, true, 4,
				 AVAGO_SERDES_BROADCAST, AVAGO_SERDES,
				 AVAGO_M4, AVAGO_P1))
		return 0;

	param &= 0xffff;
	int_num |= add_int_slice_sel(ah, sbus_addr, int_num);
	sbus_addr &= 0xffff;

	if (ah->serdes_int_fn)
		data = ah->serdes_int_fn(ah, sbus_addr, int_num, param);
	else
		debug("%s: No SerDes int function registered!\n", __func__);

	avago_log_printf(ah, AVAGO_DEBUG7, __func__, __LINE__,
			"SBus %s, loops: %d, int: 0x%02x 0x%04x -> 0x%04x.\n",
			avago_addr_to_str(sbus_addr), loops, int_num, param,
			data);
	return (data & 0xffff);
}

static int spico_int_array_nobatch(struct avago_hdl *ah, uint32_t sbus_addr,
				   int num_elements,
				   struct avago_spico_int * ints)
{
	int i;
	int return_code = ah->return_code;
	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			"num_elements = %d\n", num_elements);
	for (i = 0; i < num_elements; i++) {
		struct avago_spico_int *it = &ints[i];
		if (it->flags & AVAGO_SPICO_INT_NOT_FIRST) {
			it->ret = 0;
			continue;
		}
		it->ret = avago_spico_int(ah, sbus_addr,
					  it->interrupt, it->param);
		if (ah->return_code != return_code)
			break;
	}
	return ah->return_code == return_code ? 0 : -1;
}

/** @cond INTERNAL */

int avago_spico_int_array(struct avago_hdl *ah, uint32_t sbus_addr, int num_elements,
			  struct avago_spico_int * ints)
{
	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			"SBus %s, num_elements = %d\n",
			avago_addr_to_str(sbus_addr), num_elements);

	if (!avago_check_process(ah, sbus_addr, __func__, __LINE__, true, 3,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F, AVAGO_PROCESS_A))
		return 0;
	if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, true, 4,
				 AVAGO_SPICO, AVAGO_SERDES, AVAGO_M4, AVAGO_P1))
		return 0;

	if (!avago_get_spico_running_flag(ah, sbus_addr))
		avago_set_spico_running_flag(ah, sbus_addr, avago_spico_running(ah, sbus_addr));

	if (!avago_get_spico_running_flag(ah, sbus_addr)) {
		avago_fail(ah, __func__, __LINE__,
			  "Interrupt array not executed because SPICO not running on SBus address %s.\n",
			  avago_addr_to_str(sbus_addr));
		return ah->return_code;
	}

	spico_int_array_nobatch(ah, sbus_addr, num_elements, ints);

	return ah->return_code;
}

/** @endcond */

/** @brief    Issue the provided interrupt to a SBM SPICO. */
static uint32_t avago_sbm_spico_int(struct avago_hdl *ah,
			/**< [in] Pointer to AAPL structure */
				uint32_t sbus_addr,
			/**< [in] SBus address of SBus Master SPICO */
				int int_num,
			/**< [in] Interrupt code */
				int param)
{
/**< [in] Interrupt data */
	int loops;
	int int_data;
	uint32_t data = 0;
	param &= 0xffff;

	if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, true, 1,
				 AVAGO_SPICO))
		return 0;

	avago_sbus_wr(ah, sbus_addr, 0x02, (param << 16) | int_num);
	int_data = avago_sbus_rd(ah, sbus_addr, 0x07);

	if (avago_check_process(ah, sbus_addr, __func__, __LINE__, false, 1,
				AVAGO_PROCESS_A))
		int_data = int_data | 0x02;

	int_data = int_data | 0x01;
	avago_sbus_wr(ah, sbus_addr, 0x07, int_data);
	int_data = int_data ^ 0x01;
	avago_sbus_wr(ah, sbus_addr, 0x07, int_data);

	loops = 0;

	if (avago_get_ip_rev(ah, avago_make_sbus_controller_addr(sbus_addr))
	    <= 0xbd) {
		for (loops = 0; loops <= ah->serdes_int_timeout; loops++) {
			if (avago_sbus_rd(ah, sbus_addr, 0x08) & 0x3ff)
				break;
			if (loops > 10)
				mdelay(1);
		}
	} else {
		for (loops = 0; loops <= ah->serdes_int_timeout; loops++) {
			if ((avago_sbus_rd(ah, sbus_addr, 0x08) & 0x8000) == 0)
				break;
			if (loops > 10)
				mdelay(1);
		}
	}

	if (loops >= ah->serdes_int_timeout) {
		avago_set_spico_running_flag(ah, sbus_addr, 0);
		avago_fail(ah, __func__, __LINE__,
			  "Interrupt 0x%02x,0x%04x timed out after %d loops on SBus address %s -> 0x%x.\n",
			  int_num, param, ah->serdes_int_timeout,
			  avago_addr_to_str(sbus_addr), data);
		return 0;
	}

	data = avago_sbus_rd(ah, sbus_addr, 0x08);

	avago_log_printf(ah, AVAGO_DEBUG7, __func__, __LINE__,
			"SBus %s, loops: %d, int: 0x%02x 0x%04x -> 0x%04x.\n",
			avago_addr_to_str(sbus_addr), loops, int_num, param,
			(data >> 16) & 0xffff);
	if ((data & 0x7fff) == 1)
		return (data >> 16) & 0xffff;
	else
		return ((data >> 16) & 0xffff) | ((data & 0x7fff) << 16);
}

/**
 * @brief  Issue the interrupt to the SPICO processor, and verifies that the
 *         return value matches the interrupt number.
 *
 * @param[in]	ah	Avago handle
 * @param[in]	caller	Caller function name
 * @param[in]	line	Caller's line number
 * @param	addr	SBus address to send interrupt to
 * @param	int_num	Interrupt number
 * @param	param	Interrupt parameter to pass
 *
 * @return true for pass
 * @return false for fail
 */
bool avago_spico_int_check(struct avago_hdl *ah,
			   const char *caller,
			   int line,
			   uint32_t addr,
			   int int_num,
			   int param)
{
	int rc = avago_spico_int(ah, addr, int_num, param);

	if ((rc & 0xff) != (int_num & 0xff)) {
		avago_fail(ah, caller, line,
			  "spico_int returned incorrect value. SBus %s, spico_int(0x%x, 0x%x) returned 0x%x\n",
			  avago_addr_to_str(addr), int_num, param, rc);
		return false;
	}
	return true;
}

/**
 * @brief  Issue the interrupt to the SPICO processor.
 *
 * @param[in]	ah	Avago handle
 * @param	sbus_addr	Address to send interrupt to
 * @param	int_num	Interrupt number
 * @param	param	Interrupt parameter to pass
 *
 * @return The return value depends on the interrupt.
 *         For 28nm, it is often the same as the interrupt number.
 *         See the firmware documentation for details.
 */
uint32_t avago_spico_int(struct avago_hdl *ah,
			 uint32_t sbus_addr,
			 int int_num,
			 int param)
{
	bool st;
	int rc = 0;
	int __attribute__((unused)) return_code = ah->return_code;
	enum avago_ip_type ip_type;

	struct avago_addr addr_struct, start, stop, next;
	avago_addr_to_struct(sbus_addr, &addr_struct);
	for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		uint32_t sbus_addr = avago_struct_to_addr(&next);
		ip_type = avago_get_ip_type(ah, sbus_addr);
		if (ip_type != AVAGO_SERDES &&
		    ip_type != AVAGO_M4 &&
		    ip_type != AVAGO_P1 &&
		    ip_type != AVAGO_SPICO)
			continue;

		if (!avago_get_spico_running_flag(ah, sbus_addr))
			avago_set_spico_running_flag(ah, sbus_addr,
						     avago_spico_running(ah,
									 sbus_addr));

		if (!avago_get_spico_running_flag(ah, sbus_addr)) {
			avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
					"SBus %s, Interrupt 0x%02x,0x%04x not executed because SPICO not running.\n",
					avago_addr_to_str(sbus_addr), int_num,
					param);
			return 0;
		}
	}

	switch (avago_get_process_id(ah, sbus_addr)) {
	case AVAGO_PROCESS_B:
	case AVAGO_PROCESS_F:
		switch (avago_get_ip_type(ah, sbus_addr)) {
		case AVAGO_SERDES_BROADCAST:
		case AVAGO_P1:
		case AVAGO_M4:
		case AVAGO_SERDES:
			rc = avago_serdes_spico_int(ah, sbus_addr, int_num,
						    param);
			break;
		case AVAGO_SPICO:
			rc = avago_sbm_spico_int(ah, sbus_addr, int_num,
						 param);
			break;
		default:
			break;
		}
		break;

	case AVAGO_PROCESS_A:
		if (avago_get_ip_type(ah, sbus_addr) == AVAGO_SPICO)
			rc = avago_sbm_spico_int(ah, sbus_addr, int_num,
						 param);
		break;

	default:
		avago_fail(ah, __func__, __LINE__,
			  "SBus %s, IP type 0x%x, in process %s, is not supported.\n",
			  avago_addr_to_str(sbus_addr), avago_get_ip_type(ah,
									sbus_addr),
			  avago_get_process_id_str(ah, sbus_addr));
		return 0;
		break;
	}

	return rc;
}

uint32_t avago_spico_broadcast_int(struct avago_hdl *ah, int int_num, int param,
				   int args, ...)
{
	int i;
	va_list sbus_rx_list;
	va_start(sbus_rx_list, args);

	for (i = 0; i < args; i++)
		avago_spico_int(ah, va_arg(sbus_rx_list, uint32_t), int_num,
				param);

	va_end(sbus_rx_list);
	return ah->return_code;
}

uint32_t avago_spico_broadcast_int_w_mask(struct avago_hdl *ah, uint32_t addr_mask,
				      int int_num, int param, int args, ...)
{
	int i;
	va_list sbus_rx_list;
	va_start(sbus_rx_list, args);

	for (i = 0; i < args; i++) {
		uint32_t this_addr = va_arg(sbus_rx_list, uint);
		if ((addr_mask & (1U << i)) == (1U << i)) {
			avago_spico_int(ah, this_addr, int_num, param);
		}
	}

	va_end(sbus_rx_list);
	return ah->return_code;
}

#if AVAGO_ALLOW_MDIO || AVAGO_ALLOW_GPIO_MDIO
static void mdio_burst_upload(struct avago_hdl *ah, struct avago_addr addr_struct,
			      int words, const int rom[])
{
	uint32_t burst_word = 0x00000000;
	int word, burst_cnt = 0;

	if (addr_struct.sbus == 0xfd) {
		avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD, 32779, 0x14fd);
		avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD, 32778, 0x0001);
	} else {
		avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD, 32781,
			      (0x0a00 | (0x00ff & addr_struct.sbus)));
		avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD, 32780, 0x0001);
	}
	avago_mdio(ah, AVAGO_MDIO_ADDR, addr_struct.chip, AVSP_DEVAD, 32771);

	for (word = 0; word < words; word++) {
		burst_word = burst_word | (rom[word] << (10 * burst_cnt));
		if (burst_cnt == 2) {
			burst_word = burst_word | 0xc0000000;
			avago_mdio(ah, AVAGO_MDIO_WRITE, addr_struct.chip,
				   AVSP_DEVAD, (burst_word & 0x0000ffff));
			avago_mdio(ah, AVAGO_MDIO_WRITE, addr_struct.chip,
				   AVSP_DEVAD, ((burst_word & 0xffff0000) >> 16));
			burst_cnt = 0;
			burst_word = 0x00000000;
		} else
			burst_cnt++;
	}

	if (burst_cnt != 0) {
		burst_word = burst_word | (burst_cnt << 30);
		avago_mdio(ah, AVAGO_MDIO_WRITE, addr_struct.chip, AVSP_DEVAD,
			   (burst_word & 0x0000ffff));
		avago_mdio(ah, AVAGO_MDIO_WRITE, addr_struct.chip, AVSP_DEVAD,
			   ((burst_word & 0xffff0000) >> 16));
	}

	if (addr_struct.sbus == 0xfd)
		avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD, 32778, 0x0000);
	else
		avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD, 32780, 0x0000);
}
#endif

static void spico_burst_upload(struct avago_hdl *ah, uint32_t sbus, uint32_t reg,
			       uint32_t rom_size, const int *rom)
{
	uint32_t word;
	avago_log_printf(ah, AVAGO_DEBUG7, __func__, __LINE__,
			"sbus=0x%02x, reg=0x%x, rom_size=%u\n", sbus, reg,
			rom_size);
	for (word = 0; word < rom_size - 2; word += 3)
		avago_sbus_wr(ah, sbus, reg,
			      0xc0000000 | rom[word] | (rom[word + 1] << 10) |
			      (rom[word + 2] << 20));
	if (rom_size - word == 2)
		avago_sbus_wr(ah, sbus, reg,
			      0x80000000 | rom[word] | (rom[word + 1] << 10));
	else if (rom_size - word == 1)
		avago_sbus_wr(ah, sbus, reg, 0x40000000 | rom[word]);
}

static void spico_burst_upload_u32(struct avago_hdl *ah, uint32_t sbus,
				   uint32_t reg, uint32_t rom_size,
				   const uint32_t *rom)
{
	uint32_t word;
	avago_log_printf(ah, AVAGO_DEBUG7, __func__, __LINE__,
			 "sbus=%s, reg=0x%x, rom_size=%u words\n",
			 avago_addr_to_str(sbus), reg, rom_size);
	debug("%s(0x%p, %s, 0x%x, 0x%x, 0x%p)\n", __func__, ah,
	      avago_addr_to_str(sbus), reg, rom_size, rom);
	for (word = 0; word < rom_size; word++)
		avago_sbus_wr(ah, sbus, reg, rom[word]);
	debug("%s: Done.\n", __func__);
}

/**
 * @brief   Internal function that uploads the ROM blindly to the sbus_addr.
 * @return  On success, returns 0.
 * @return  On error, decrements ah->return_code and returns -1.
 */
static int spico_upload_image(struct avago_hdl *ah, uint32_t sbus_addr,
			      int words, const int rom[])
{
	int return_code = ah->return_code;

	if (avago_check_process(ah, sbus_addr, __func__, __LINE__, false, 2,
				AVAGO_PROCESS_B, AVAGO_PROCESS_F)) {
		struct avago_addr addr_struct;
		avago_addr_to_struct(sbus_addr, &addr_struct);
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 6, AVAGO_SERDES,
					AVAGO_SERDES_D6_BROADCAST, AVAGO_M4,
					AVAGO_SERDES_M4_BROADCAST, AVAGO_P1,
					AVAGO_SERDES_P1_BROADCAST)) {

			avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000011);
			avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000010);
			avago_sbus_wr(ah, sbus_addr, 0x08, 0x00000030);
			avago_sbus_wr(ah, sbus_addr, 0x00, 0xc0000000);

#if AVAGO_ALLOW_MDIO || AVAGO_ALLOW_GPIO_MDIO
			if (avago_is_mdio_communication_method(ah))
				mdio_burst_upload(ah, addr_struct, words, rom);
			else
#endif
			{
				if (avago_get_lsb_rev(ah, sbus_addr) <= 2) {
					int word;
					debug("Performing non-burst upload\n");
					for (word = 0; word < words; word++)
						avago_sbus_wr(ah, sbus_addr,
							      0x00,
							      0xc0000000 |
							      (rom[word] << 16)
							      | word);
				} else {
					debug("%s: Performing burst upload\n",
					      __func__);
					spico_burst_upload(ah, sbus_addr,
							   0x0a, words, rom);
				}
			}

			avago_sbus_wr(ah, sbus_addr, 0x00, 0x00000000);
			avago_sbus_wr(ah, sbus_addr, 0x0b, 0x000c0000);
			avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000002);
			avago_sbus_wr(ah, sbus_addr, 0x08, 0x00000000);
		} else if (avago_check_ip_type(ah, sbus_addr, __func__,
					       __LINE__, true, 2, AVAGO_SPICO,
					       AVAGO_SPICO_BROADCAST)) {
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x000000c0);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000040);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000240);

#if AVAGO_ALLOW_MDIO || AVAGO_ALLOW_GPIO_MDIO
			if (avago_is_mdio_communication_method(ah))
				mdio_burst_upload(ah, addr_struct, words, rom);
			else
#endif
			{
				int rev =
				    avago_get_ip_rev(ah,
					avago_make_sbus_controller_addr(sbus_addr));
				if (rev >= 0xbe ||
				    avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_B) {
					avago_sbus_wr(ah, sbus_addr, 0x03,
						      0x00000000);
					avago_sbus_wr(ah, sbus_addr, 0x03,
						      0x80000000);
					spico_burst_upload(ah, sbus_addr,
							   0x14, words, rom);
				} else {

					int word;
					for (word = 0; word < words; word++)
						avago_sbus_wr(ah, sbus_addr,
							      0x03,
							      0x80000000 |
							      (rom[word] << 16)
							      | word);
				}
			}

			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000040);
			avago_sbus_wr(ah, sbus_addr, 0x16, 0x000c0000);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000140);
		}
	} else if (avago_check_process(ah, sbus_addr, __func__, __LINE__, true,
				       1, AVAGO_PROCESS_A)) {
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					true, 1, AVAGO_SPICO)) {
			int word;
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x000000c0);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000040);
			avago_sbus_wr(ah, sbus_addr, 0x03, 0x40000000);

			for (word = 0; word < words; word++)
				avago_sbus_wr(ah, sbus_addr, 0x03,
					      0xc0000000 | (rom[word] << 16) |
					      word);

			avago_sbus_wr(ah, sbus_addr, 0x03, 0x00000000);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000140);
			avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000002);
		}
	}
	return ah->return_code == return_code ? 0 : -1;
}

/**
 * @brief   Internal function that uploads the ROM blindly to the sbus_addr.
 * @return  On success, returns 0.
 * @return  On error, decrements ah->return_code and returns -1.
 */
static int spico_upload_image_u32(struct avago_hdl *ah, uint32_t sbus_addr,
				  int words, const uint32_t rom[])
{
	int return_code = ah->return_code;

	if (avago_check_process(ah, sbus_addr, __func__, __LINE__, false, 2,
				AVAGO_PROCESS_B, AVAGO_PROCESS_F)) {
		struct avago_addr addr_struct;
		avago_addr_to_struct(sbus_addr, &addr_struct);
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 6, AVAGO_SERDES,
					AVAGO_SERDES_D6_BROADCAST, AVAGO_M4,
					AVAGO_SERDES_M4_BROADCAST, AVAGO_P1,
					AVAGO_SERDES_P1_BROADCAST)) {
			debug("%s: 16 or 28nm, D6, M4 or P1\n", __func__);
			avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000011);
			avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000010);
			avago_sbus_wr(ah, sbus_addr, 0x08, 0x00000030);
			avago_sbus_wr(ah, sbus_addr, 0x00, 0xc0000000);

#if AVAGO_ALLOW_MDIO || AVAGO_ALLOW_GPIO_MDIO
			if (avago_is_mdio_communication_method(ah))
				mdio_burst_upload_u32(ah, addr_struct, words,
						      rom);
			else
#endif
			{
				if (avago_get_lsb_rev(ah, sbus_addr) <= 2) {
					avago_fail(ah, __func__, __LINE__,
						   "Only burst upload is supported.\n");
					return -1;
				}

				debug("%s: Performing burst upload to %s\n",
				      __func__,
				      avago_addr_to_str(sbus_addr));
				spico_burst_upload_u32(ah, sbus_addr,
						       0x0a, words, rom);
			}
			avago_sbus_wr(ah, sbus_addr, 0x00, 0x00000000);
			avago_sbus_wr(ah, sbus_addr, 0x0b, 0x000c0000);
			avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000002);
			avago_sbus_wr(ah, sbus_addr, 0x08, 0x00000000);
		} else if (avago_check_ip_type(ah, sbus_addr, __func__,
					       __LINE__, true, 2, AVAGO_SPICO,
					       AVAGO_SPICO_BROADCAST)) {
			debug("%s: Spico or Spico broadcast\n", __func__);

			avago_sbus_wr(ah, sbus_addr, 0x01, 0x000000c0);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000040);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000240);

#if AVAGO_ALLOW_MDIO || AVAGO_ALLOW_GPIO_MDIO
			if (avago_is_mdio_communication_method(ah))
				mdio_burst_upload_u32(ah, addr_struct, words, rom);
			else
#endif
			{
				int rev = avago_get_ip_rev(ah,
						avago_make_sbus_controller_addr(sbus_addr));
				if (rev >= 0xbe ||
				    avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_B) {
					avago_sbus_wr(ah, sbus_addr, 0x03,
						      0x00000000);
					avago_sbus_wr(ah, sbus_addr, 0x03,
						      0x80000000);
					spico_burst_upload_u32(ah, sbus_addr,
							       0x14, words, rom);
				} else {
					avago_fail(ah, __func__, __LINE__,
						   "Non-burst upload not supported.\n");
					return -1;
				}
			}

			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000040);
			avago_sbus_wr(ah, sbus_addr, 0x16, 0x000c0000);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000140);
		}
	} else if (avago_check_process(ah, sbus_addr, __func__, __LINE__, true,
				       1, AVAGO_PROCESS_A)) {
		avago_fail(ah, __func__, __LINE__,
			   "Avago 40nm process not supported.\n");
		return -1;
	}
	return ah->return_code == return_code ? 0 : -1;
}

void avago_crc_one_byte(int *crc_ptr, int value)
{
	int crc = *crc_ptr;
	crc += value;
	crc ^= 0xd8;
	if (crc & 0x80000000)
		crc = (crc << 1) + 229;
	else
		crc <<= 1;
	*crc_ptr = crc;
}

int avago_crc_rom(int *memory, int length)
{
	int i, crc = 0;
	for (i = 0; i < length; i++)
		avago_crc_one_byte(&crc, memory[i]);
	return crc ^ 0xdeadbeef;
}

/** @endcond */

/** @brief   Upload SPICO machine code and check CRC. */
/**          Works for broadcast and individual addresses. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
int avago_spico_upload(struct avago_hdl *ah,
			    /**< Pointer to Aapl_t structure */
		       uint32_t sbus_addr_in,
			    /**< Sbus address */
		       bool ram_bist,
			    /**< If true, perform spico_ram_bist prior to upload */
		       int words,
			    /**< Length of ROM image */
		       const int *rom)
{
/**< Avago-supplied ROM image */
	int return_code = ah->return_code;
	bool st;
	struct avago_addr addr_struct, start, stop, next;

	if (!avago_check_ip_type(ah, sbus_addr_in, __func__, __LINE__, true, 8,
				 AVAGO_SPICO, AVAGO_SERDES,
				 AVAGO_SERDES_BROADCAST, AVAGO_SPICO_BROADCAST,
				 AVAGO_M4, AVAGO_SERDES_M4_BROADCAST, AVAGO_P1,
				 AVAGO_SERDES_P1_BROADCAST)
	    || !avago_check_process(ah, sbus_addr_in, __func__, __LINE__, true,
				    3, AVAGO_PROCESS_A, AVAGO_PROCESS_F,
				    AVAGO_PROCESS_B))
		return 0;

	if (words <= 0) {
		avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
				"0 byte image specified. Skipping upload to %s.\n",
				avago_addr_to_str(sbus_addr_in));
		return 0;
	}

	avago_addr_to_struct(sbus_addr_in, &addr_struct);

	for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		bool ignore = false;
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		if (!avago_addr_selects_fw_device(ah, &addr_struct, sbus_addr,
						  &ignore)) {
			if (ignore)
				avago_log_printf(ah, AVAGO_DEBUG2, __func__,
						__LINE__,
						"Skipping reset of SBus address %s because an upload was requested to %s and the ignore broadcast bit on this SerDes was set.\n",
						avago_addr_to_str(sbus_addr),
						avago_addr_to_str(sbus_addr_in));
			continue;
		}

		avago_sbus_reset(ah, sbus_addr, 0);
		avago_set_spico_running_flag(ah, sbus_addr, 0);
		if (ram_bist)
			avago_spico_ram_bist(ah, sbus_addr);
	}

	avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
			"Uploading %d bytes of SPICO machine code to SBus address %s.\n",
			words, avago_addr_to_str(sbus_addr_in));
	spico_upload_image(ah, sbus_addr_in, words, rom);

	for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		bool ignore = false;
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		if (!avago_addr_selects_fw_device(ah, &addr_struct, sbus_addr,
						  &ignore)) {
			if (ignore)
				avago_log_printf(ah, AVAGO_DEBUG2, __func__,
						__LINE__,
						"Skipping CRC check of SBus address %s because an upload was requested to %s and the ignore broadcast bit on this SerDes was set.\n",
						avago_addr_to_str(sbus_addr),
						avago_addr_to_str(sbus_addr_in));
			continue;
		}
		avago_set_spico_running_flag(ah, sbus_addr, 1);
		if (avago_spico_crc(ah, sbus_addr))
			avago_set_ip_type(ah, sbus_addr);
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 1, AVAGO_SPICO)) {
			int end_addr = avago_spico_int(ah, sbus_addr, 0x1c, 0);
			avago_set_spico_running_flag(ah, sbus_addr, 1);
			if (end_addr < words) {
				int crc = avago_spico_int(ah, sbus_addr, 0x1a, 0);
				if (crc)
					avago_log_printf(ah, AVAGO_DEBUG2,
							__func__, __LINE__,
							"SBus %s, SDI CRC passed.\n",
							avago_addr_to_str
							(sbus_addr));
				else
					avago_fail(ah, __func__, __LINE__,
						  "SBus %s, SDI CRC failed. CRC interrupt returned 0x%04x\n",
						  avago_addr_to_str(sbus_addr),
						  crc);
			}
		}
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Upload SPICO machine code and check CRC. */
/**          Works for broadcast and individual addresses. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
int avago_spico_upload_u32(struct avago_hdl *ah,
		       /**< Pointer to Aapl_t structure */
		       uint32_t sbus_addr_in,
		       /**< Sbus address */
		       bool ram_bist,
		       /**< If true, perform spico_ram_bist prior to upload */
		       int words,
		       /**< Length of ROM image */
		       const uint32_t *rom)
{
	/**< Avago-supplied ROM image */
	int return_code = ah->return_code;
	bool st;
	struct avago_addr addr_struct, start, stop, next;

	debug("%s(0x%p, %s, %s, 0x%x, 0x%p)\n", __func__, ah,
	      avago_addr_to_str(sbus_addr_in),
	      ram_bist ? "true" : "false", words, rom);
	if (!avago_check_ip_type(ah, sbus_addr_in, __func__, __LINE__, true, 8,
				 AVAGO_SPICO, AVAGO_SERDES,
				 AVAGO_SERDES_BROADCAST, AVAGO_SPICO_BROADCAST,
				 AVAGO_M4, AVAGO_SERDES_M4_BROADCAST, AVAGO_P1,
				 AVAGO_SERDES_P1_BROADCAST)
	    || !avago_check_process(ah, sbus_addr_in, __func__, __LINE__, true,
				    3, AVAGO_PROCESS_A, AVAGO_PROCESS_F,
				    AVAGO_PROCESS_B))
		return 0;

	if (words <= 0) {
		avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
				 "0 word image specified. Skipping upload to %s.\n",
				 avago_addr_to_str(sbus_addr_in));
		return 0;
	}

	avago_addr_to_struct(sbus_addr_in, &addr_struct);

	for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		bool ignore = false;
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		debug("%s: Selecting address %s\n", __func__,
		      avago_addr_to_str(sbus_addr));
		if (!avago_addr_selects_fw_device(ah, &addr_struct, sbus_addr,
						  &ignore)) {
			if (ignore)
				avago_log_printf(ah, AVAGO_DEBUG2, __func__,
						 __LINE__,
						 "Skipping reset of SBus address %s because an upload was requested to %s and the ignore broadcast bit on this SerDes was set.\n",
						 avago_addr_to_str(sbus_addr),
						 avago_addr_to_str(sbus_addr_in));
			continue;
		}
		debug("%s: Resetting SBus address %s\n", __func__,
		      avago_addr_to_str(sbus_addr));
		avago_sbus_reset(ah, sbus_addr, 0);
		avago_set_spico_running_flag(ah, sbus_addr, 0);
		if (ram_bist)
			avago_spico_ram_bist(ah, sbus_addr);
	}

	avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
			 "Uploading %d words of SPICO machine code to SBus address %s.\n",
			 words, avago_addr_to_str(sbus_addr_in));
	spico_upload_image_u32(ah, sbus_addr_in, words, rom);

	for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		bool ignore = false;
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		debug("%s: Selecting sbus address 0x%x\n", __func__, sbus_addr);
		if (!avago_addr_selects_fw_device(ah, &addr_struct, sbus_addr,
						  &ignore)) {
			  if (ignore)
				  avago_log_printf(ah, AVAGO_DEBUG2, __func__,
						   __LINE__,
						   "Skipping CRC check of SBus address %s because an upload was requested to %s and the ignore broadcast bit on this SerDes was set.\n",
						   avago_addr_to_str(sbus_addr),
						   avago_addr_to_str(sbus_addr_in));
			  continue;
		}
		avago_set_spico_running_flag(ah, sbus_addr, 1);
		if (avago_spico_crc(ah, sbus_addr))
			avago_set_ip_type(ah, sbus_addr);
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 1, AVAGO_SPICO)) {
			int end_addr = avago_spico_int(ah, sbus_addr, 0x1c, 0);
			avago_set_spico_running_flag(ah, sbus_addr, 1);
			if (end_addr < words) {
				int crc = avago_spico_int(ah, sbus_addr, 0x1a, 0);
				if (crc)
					avago_log_printf(ah, AVAGO_DEBUG2,
							 __func__, __LINE__,
							 "SBus %s, SDI CRC passed.\n",
							 avago_addr_to_str(sbus_addr));
				else
					avago_fail(ah, __func__, __LINE__,
						   "SBus %s, SDI CRC failed. CRC interrupt returned 0x%04x\n",
						    avago_addr_to_str(sbus_addr),
						    crc);
			}
			debug("%s: end address: 0x%x\n", __func__, end_addr);
		}
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Resets and uploads all the Avago firmware for the device. */
/** @details Makes the device ready for additional configuration by performing */
/**          soft resets and sequencing the uploads for correct operation. */
/** @details The SerDes ROM image is directly loaded to the SerDes on the */
/**          addressed chip(s) and ring(s).  The SBM ROM image is directly */
/**          uploaded to the SBus Master processor.  If specified, the SDI ROM */
/**          image is appended to the SBus Master image in the target processor */
/**          memory.  If the SBM ROM image already has an SDI (with swap and/or */
/**          a state table) appended, or no SDI image is needed, the sbm_rom */
/**          pointer should be NULL. */
/** */
/**          The SDI ROM contains SerDes swap images, an AVSP state table, */
/**          or both.  Note that SerDes swap images are already in the SDI */
/**          format. */
/** */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
/** @see     avago_firmware_upload_file(). */
int avago_firmware_upload(struct avago_hdl *ah,
				    /**< Pointer to Aapl_t structure. */
			  uint32_t addr,/**< SerDes/chip/ring addresses targeted. */
			  int serdes_rom_size,
				    /**< Size of SerDes ROM image. */
			  const int *serdes_rom,
				    /**< Pointer to SerDes ROM image. */
			  int sbm_rom_size,
				    /**< Size of SBM ROM image. */
			  const int *sbm_rom,
				    /**< Pointer to SBM ROM image. */
			  int sdi_rom_size,
				    /**< Size of sdi (swap) ROM image. */
			  const int *sdi_rom)
{
/**< Pointer to sdi (swap) ROM image. */
	int return_code = ah->return_code;
	uint32_t sbm_addr = avago_make_sbus_master_addr(addr);

	avago_spico_upload(ah, addr, true, serdes_rom_size, serdes_rom);
	if (0 == avago_spico_upload(ah, sbm_addr, true, sbm_rom_size, sbm_rom)
	    && sdi_rom && sdi_rom_size > 0) {
		if (avago_get_ip_rev(ah, avago_make_sbus_controller_addr(addr))
		    >= 0x00be) {
			int end_addr = avago_spico_int(ah, sbm_addr, 0x1c, 0);
			if (end_addr == sbm_rom_size)
				avago_spico_upload_swap_image(ah, addr,
							      sdi_rom_size,
							      sdi_rom);
			else
				avago_log_printf(ah, AVAGO_WARNING, __func__,
						__LINE__,
						"Skipping swap/SDI ROM upload because SBus Master image already contains an SDI image.\n");
		} else {
			avago_spico_upload_swap_image(ah, addr, sdi_rom_size,
						      sdi_rom);
		}
	}

	return return_code == ah->return_code ? 0 : -1;
}

/**
 * @brief   Resets and uploads all the Avago firmware for the device.
 *
 * @param[in]	ah		Avago handle
 * @param	addr		SerDes/Chip/Ring addresses targeted
 * @param	serdes_rom_size	Size (in words) of SerDes ROM
 * @param[in]	serdes_rom	Pointer to array of words containing the ROM
 * @param	sbm_rom_size	Size (in words) of SBM ROM image
 * @param[in]	sbm_rom		Pointer to array of words containing SBM ROM
 *
 * @details Makes the device ready for additional configuration by performing
 *          soft resets and sequencing the uploads for correct operation.
 * @details The SerDes ROM image is directly loaded to the SerDes on the
 *          addressed chip(s) and ring(s).  The SBM ROM image is directly
 *          uploaded to the SBus Master processor.  If specified, the SDI ROM
 *          image is appended to the SBus Master image in the target processor
 *          memory.
 *
 * NOTE: This version of this function does not handle SDI ROM or swap ROM
 *	 images.
 *
 * @return  On success, returns 0.
 * @return  On error, decrements ah->return_code and returns -1.
 * @see     avago_firmware_upload_file().
 */
int avago_firmware_upload_u32(struct avago_hdl *ah,
			      uint32_t addr,
			      int serdes_rom_size,
			      const uint32_t *serdes_rom,
			      int sbm_rom_size,
			      const uint32_t *sbm_rom)
{
	/**< Pointer to sdi (swap) ROM image. */
	int return_code = ah->return_code;
	uint32_t sbm_addr = avago_make_sbus_master_addr(addr);

	avago_spico_upload_u32(ah, addr, true, serdes_rom_size, serdes_rom);
	avago_spico_upload_u32(ah, sbm_addr, true, sbm_rom_size, sbm_rom);

	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Runs RAM BIST on given SBus address. */
/** @details Works for broadcast and individual addresses. */
/** @return  On success, returns 0. */
/**          On error, decrements ah->return_code and returns -1. */
int avago_spico_ram_bist(struct avago_hdl *ah,
				/**< Pointer to Aapl_t structure */
			 uint32_t sbus_addr_in)
{
/**< Sbus address */
	int return_code = ah->return_code;
	int loops;
	uint32_t data = 0;
	bool st;
	struct avago_addr addr_struct, start, stop, next;

	if (!avago_check_ip_type(ah, sbus_addr_in, __func__, __LINE__, true, 8,
				 AVAGO_SERDES, AVAGO_SPICO,
				 AVAGO_SERDES_BROADCAST, AVAGO_SPICO_BROADCAST,
				 AVAGO_M4, AVAGO_SERDES_M4_BROADCAST, AVAGO_P1,
				 AVAGO_SERDES_P1_BROADCAST)
	    || !avago_check_process(ah, sbus_addr_in, __func__, __LINE__, true,
				   3, AVAGO_PROCESS_B, AVAGO_PROCESS_F,
				   AVAGO_PROCESS_A))
		return 0;

	avago_addr_to_struct(sbus_addr_in, &addr_struct);
	for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		uint32_t rambist_mask = 0;
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 1, AVAGO_SPICO)) {
			avago_sbus_wr(ah, sbus_addr, 0x00, 0x03);
			avago_set_spico_running_flag(ah, sbus_addr, 0);
			avago_sbus_rmw(ah, sbus_addr, 0x01, 0x080, 0x180);
			avago_sbus_wr(ah, sbus_addr, 0x00, 0x05);
			for (loops = 0;
			     loops <= ah->serdes_int_timeout;
			     loops++) {
				data = avago_sbus_rd(ah, sbus_addr, 0x00);
				if (data & 0x18)
					break;
			}

			if (loops >= ah->serdes_int_timeout)
				avago_fail(ah, __func__, __LINE__,
					  "SBus master SPICO RAM BIST timed out on SBus address %s -> 0x%x.\n",
					  avago_addr_to_str(sbus_addr), data);
			else if ((data & 0x18) == 0x08)
				avago_log_printf(ah, AVAGO_DEBUG1, __func__,
						__LINE__,
						"SBus master SPICO RAM BIST test on SBus address %s passed.\n",
						avago_addr_to_str(sbus_addr));
			else
				avago_fail(ah, __func__, __LINE__,
					  "SBus master SPICO RAM BIST on SBus %s failed -> 0x%x.\n",
					  avago_addr_to_str(sbus_addr), data);

			avago_sbus_wr(ah, sbus_addr, 0x00, 0x01);
			continue;
		}

		if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					 false, 3, AVAGO_SERDES, AVAGO_M4,
					 AVAGO_P1))
			continue;

		if (start.sbus != stop.sbus
		    && (avago_sbus_rd(ah, sbus_addr, 0xfd) & 0x1) == 0x1) {
			avago_log_printf(ah, AVAGO_DEBUG2, __func__, __LINE__,
					"Skipping RAM BIST check of SBus address %s because a RAM BIST test was requested to %s and the ignore broadcast bit on this serdes was set.\n",
					avago_addr_to_str(sbus_addr),
					avago_addr_to_str(sbus_addr_in));
			continue;
		}

		if ((avago_get_lsb_rev(ah, sbus_addr) < 5)
		    && (avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_F))
			rambist_mask = 8;

		avago_sbus_wr(ah, sbus_addr, 0x09, 0x00 | rambist_mask);
		data = avago_sbus_rd(ah, sbus_addr, 0x09);

		if ((data & 0x001f) != rambist_mask) {
			avago_fail(ah, __func__, __LINE__,
				  "0x%02x, RAM BIST setup failed. Returned value: 0x%08x, expecting: 0x%08x.\n",
				  sbus_addr, data, rambist_mask);
			avago_sbus_wr(ah, sbus_addr, 0x09, rambist_mask);
			continue;
		}

		avago_log_printf(ah, AVAGO_DEBUG6, __func__, __LINE__,
				"Performing RAM BIST check of SBus address %s because a RAM BIST test was requested to %s.\n",
				avago_addr_to_str(sbus_addr),
				avago_addr_to_str(sbus_addr_in));
		avago_set_spico_running_flag(ah, sbus_addr, 0);
		avago_sbus_wr(ah, sbus_addr, 0x07, 0x11);
		avago_sbus_wr(ah, sbus_addr, 0x09, 0x03 | rambist_mask);

		for (loops = 0; loops <= ah->serdes_int_timeout; loops++) {
			data = avago_sbus_rd(ah, sbus_addr, 0x09);
			if (data & 0x3f0060)
				break;

			if (avago_get_lsb_rev(ah, sbus_addr) >= 3
			    && avago_get_lsb_rev(ah, sbus_addr) < 8) {
				if ((data & 0x3f00) == 0x0300)
					break;
			}
		}

		if (loops >= ah->serdes_int_timeout)
			avago_fail(ah, __func__, __LINE__,
				  "SBus %s, RAM BIST timed out -> 0x%x\n",
				  avago_addr_to_str(sbus_addr), data);
		else if ((data & 0x3f0040) != 0x0)
			avago_fail(ah, __func__, __LINE__,
				  "SBus %s, RAM BIST failed -> 0x%x.\n",
				  avago_addr_to_str(sbus_addr), data);
		else
			avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
					"SBus %s, RAM BIST test passed.\n",
					avago_addr_to_str(sbus_addr));

		avago_sbus_wr(ah, sbus_addr, 0x09, rambist_mask);
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief  Gets the revision of the firmware loaded into the SPICO processor. */
/** @return  On success, returns the firmware revision. */
/**          If SPICO is stopped, returns 0. */
/** @see avago_firmware_get_build_id(). */
uint32_t avago_firmware_get_rev(struct avago_hdl *ah,
			/**< Pointer to Aapl_t structure */
			    uint32_t addr)
{
/**< Sbus address of the SPICO to check. */
	uint32_t rc = 0;
	enum avago_process_id process_id = avago_get_process_id(ah, addr);
	if (process_id == AVAGO_PROCESS_F || process_id == AVAGO_PROCESS_B) {
		if (avago_check_ip_type(ah, addr, __func__, __LINE__, true, 4,
					AVAGO_SERDES, AVAGO_SPICO, AVAGO_M4,
					AVAGO_P1))
			rc = avago_spico_int(ah, addr, 0, 0);
	} else if (process_id == AVAGO_PROCESS_A) {
		if (avago_check_ip_type(ah, addr, __func__, __LINE__, true, 1,
					AVAGO_SPICO))
			rc = avago_spico_int(ah, addr, 1, 0);
	}
	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			"SBus %s, fw_rev = 0x%04x.\n", avago_addr_to_str(addr),
			rc);
	return rc;
}

/** @brief  Gets the build id of the firmware loaded into the SPICO processor. */
/** @return On success, returns the firmware build id. */
/**         If SPICO is stopped, returns 0. */
/** @see    avago_firmware_get_rev(). */
uint32_t avago_firmware_get_build_id(struct avago_hdl *ah,
			/**< Pointer to Aapl_t structure */
				 uint32_t addr)
{
/**< Sbus address of the SPICO to check. */
	uint32_t rc = 0;
	enum avago_process_id process_id = avago_get_process_id(ah, addr);
	if (process_id == AVAGO_PROCESS_F || process_id == AVAGO_PROCESS_B) {
		if (avago_check_ip_type(ah, addr, __func__, __LINE__, true, 4,
					AVAGO_SERDES, AVAGO_SPICO, AVAGO_M4,
					AVAGO_P1)) {
			if (avago_check_ip_type(ah, addr, __func__, __LINE__,
						false, 3, AVAGO_SERDES,
						AVAGO_M4, AVAGO_P1))
				rc = avago_spico_int(ah, addr, 0x3f, 0);
			else
				rc = avago_spico_int(ah, addr, 0x01, 0);
		}
	} else if (process_id == AVAGO_PROCESS_A) {
		if (avago_check_ip_type(ah, addr, __func__, __LINE__, true, 1,
					AVAGO_SPICO))
			rc = avago_spico_int(ah, addr, 0x00, 0);
	}
	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			"SBus %s, fw_build_id = %04X.\n",
			avago_addr_to_str(addr), rc);
	return rc;
}

/** @brief  Return the CRC checksum of the firmware image on sbus_addr. */
/** */
/** @return 0 - CRC checksum failed */
/** @return 1 - CRC checksum passed */
uint32_t avago_spico_crc(struct avago_hdl *ah,
			    /**< Pointer to Aapl_t structure */
		     uint32_t sbus_addr)
{
/**< Sbus address */
	int crc_status = 0;
	int crc = 1;
	int return_code = ah->return_code;
	int running;

	if (!avago_check_process(ah, sbus_addr, __func__, __LINE__, true, 3,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F,
				 AVAGO_PROCESS_A))
		return 0;

	switch (avago_get_process_id(ah, sbus_addr)) {
	case AVAGO_PROCESS_B:
	case AVAGO_PROCESS_F:
		if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					 true, 7, AVAGO_SERDES,
					 AVAGO_SERDES_BROADCAST, AVAGO_SPICO,
					 AVAGO_M4, AVAGO_SERDES_M4_BROADCAST,
					 AVAGO_P1, AVAGO_SERDES_P1_BROADCAST)) {
			return 0;
		}
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 6, AVAGO_SERDES,
					AVAGO_SERDES_BROADCAST, AVAGO_M4,
					AVAGO_SERDES_M4_BROADCAST, AVAGO_P1,
					AVAGO_SERDES_P1_BROADCAST)) {
			crc = avago_spico_int(ah, sbus_addr, 0x3c, 0);
			crc_status = crc == 0;
		} else if (avago_check_ip_type(ah, sbus_addr, __func__,
					       __LINE__, false, 1,
					       AVAGO_SPICO)) {
			crc = avago_spico_int(ah, sbus_addr, 0x2, 0);
			crc_status = crc == 1;
		}
		break;

	case AVAGO_PROCESS_A:
		if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					 true, 1, AVAGO_SPICO))
			return 0;
		crc = avago_spico_int(ah, sbus_addr, 0x02, 0);
		crc_status = crc == 1;
		break;

	default:
		break;
	}

	running = avago_get_spico_running_flag(ah, sbus_addr);
	debug("%s: return code: %d, running: %d\n", __func__,
	      ah->return_code, running);
	if (crc_status && return_code == ah->return_code && running) {
		avago_log_printf(ah, AVAGO_DEBUG2, __func__, __LINE__,
				"SBus %s, CRC passed.\n",
				avago_addr_to_str(sbus_addr));
		return 1;
	}
	avago_fail(ah, __func__, __LINE__,
		  "SBus %s, CRC failed. CRC interrupt returned 0x%04x. Running: %d.\n",
		  avago_addr_to_str(sbus_addr), crc, running);
	return 0;
}

/**
 * @brief   Resets a SerDes SPICO processor.
 *
 * @param	ah	Avago handle
 * @param	sbus_addr	SBus slice address
 *
 * @details Supports broadcast addresses.
 * @return  On success, returns 0.
 * @return  On error, decrements ah->return_code and returns -1.
 */
int avago_spico_reset(struct avago_hdl *ah, uint32_t sbus_addr)
{
	int return_code = ah->return_code;
	if (!avago_check_process(ah, sbus_addr, __func__, __LINE__, true, 2,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F))
		return -1;
	if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, false, 3,
				AVAGO_SERDES, AVAGO_M4, AVAGO_P1)) {
		avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000011);
		avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000010);
		avago_sbus_wr(ah, sbus_addr, 0x0b, 0x000c0000);
		avago_sbus_wr(ah, sbus_addr, 0x07, 0x00000002);
		avago_sbus_wr(ah, sbus_addr, 0x08, 0x00000000);
	} else if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, true,
				       1, AVAGO_SPICO)) {
		avago_sbus_wr(ah, sbus_addr, 0x01, 0x000000c0);
		avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000040);
		avago_sbus_wr(ah, sbus_addr, 0x16, 0x000c0000);
		avago_sbus_wr(ah, sbus_addr, 0x01, 0x00000140);
	}
	return return_code == ah->return_code ? 0 : -1;
}

/** @brief   Check to see if any uploads are in progress, and waits for them to complete */
/** */
void avago_twi_wait_for_complete(struct avago_hdl *ah,
			 /**< Pointer to Aapl_t structure */
				 uint32_t sbus_addr)
{
/**< Sbus address of device to check for current upload */
	if (!avago_check_process(ah, sbus_addr, __func__, __LINE__, true, 3,
				 AVAGO_PROCESS_B,  AVAGO_PROCESS_F, AVAGO_PROCESS_A) ||
	    avago_check_broadcast_address(ah, sbus_addr, __func__, __LINE__,
					  true) ||
	    !avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, true, 2,
				 AVAGO_SBUS_CONTROLLER))
		return;

	if (avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_A)
		return;

	if (avago_get_ip_rev(ah, avago_make_sbus_controller_addr(sbus_addr) == 0xbf)) {
		int initial = avago_sbus_rmw(ah, sbus_addr, 0x40, 1 << 13, 1 << 13);
		int twi_status = avago_sbus_rd(ah, sbus_addr, 0x81);
		avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
				"TWI status: 0x%08x.\n", twi_status);

		if (((twi_status >> 18) & 0x3) == 0x1) {
			int loops;
			avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
					"SPICO upload in progress on %s. Waiting for completion (0x%08x).\n",
					avago_addr_to_str(sbus_addr),
					twi_status);
			for (loops = 0; loops <= AVAGO_SPICO_UPLOAD_WAIT_TIMEOUT;
			     loops++) {
				mdelay(1);
				twi_status = avago_sbus_rd(ah, sbus_addr, 0x81);
				if (((twi_status >> 18) & 0x3) != 0x1)
					break;
			}
			if (loops >= AVAGO_SPICO_UPLOAD_WAIT_TIMEOUT)
				avago_fail(ah, __func__, __LINE__,
					  "Timed out after %d loops while waiting for TWI upload to complete (0x%02x).\n",
					  loops, twi_status);
			avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
					"Waited %d loops for pending upload to complete (0x%02x).\n",
					loops, twi_status);
		}
		avago_sbus_wr(ah, sbus_addr, 0x40, initial);
		if (((twi_status >> 18) & 0x3) == 0x2)
			avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
					"TWI status indicates failure: 0x%08x.\n",
					twi_status);
	}
}

/** @brief   Check to see if any uploads are in progress, and waits for them to complete */
/** */
void avago_spico_wait_for_upload(struct avago_hdl *ah,
			 /**< Pointer to Aapl_t structure */
				 uint32_t sbus_addr)
{
/**< Sbus address of device to check for current upload */
	uint32_t imem_cntl = 0;
	uint32_t loops;

	if (!avago_check_process(ah, sbus_addr, __func__, __LINE__, true, 2,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F) ||
	    avago_check_broadcast_address(ah, sbus_addr, __func__, __LINE__,
					  true) ||
	    !avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, true, 4,
				 AVAGO_SERDES, AVAGO_SPICO, AVAGO_M4,
				 AVAGO_P1))
		return;

	if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, false, 3,
				AVAGO_SERDES, AVAGO_M4, AVAGO_P1)) {
		imem_cntl = avago_sbus_rd(ah, sbus_addr, 0x00);
		if (imem_cntl & 0x40000000) {
			avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
					"SPICO upload in progress on %s. Waiting for completion (0x%08x).\n",
					avago_addr_to_str(sbus_addr), imem_cntl);
			for (loops = 0; loops <= AVAGO_SPICO_UPLOAD_WAIT_TIMEOUT;
			     loops++) {
				mdelay(1);
				if ((avago_sbus_rd(ah, sbus_addr, 0x07) & 0x02)
				    || !(imem_cntl & 0x40000000))
					break;
				imem_cntl =
				    avago_sbus_rd(ah, sbus_addr, 0x00);
			}
			if (loops >= AVAGO_SPICO_UPLOAD_WAIT_TIMEOUT)
				avago_fail(ah, __func__, __LINE__,
					  "Timed out after %d loops while waiting for SPICO upload to complete (0x%02x 0x%08x).\n",
					  loops, avago_sbus_rd(ah, sbus_addr,
							       0x07),
					  imem_cntl);
			avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
					"Waited %d loops for pending upload to complete (0x%02x 0x%08x).\n",
					loops, avago_sbus_rd(ah, sbus_addr,
							     0x07), imem_cntl);
		}
	} else if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, false,
				       1, AVAGO_SPICO)) {
		imem_cntl = avago_sbus_rd(ah, sbus_addr, 0x01);
		if (imem_cntl & 0x200) {
			for (loops = 0; loops <= AVAGO_SPICO_UPLOAD_WAIT_TIMEOUT;
			     loops++) {
				mdelay(1);
				if ((imem_cntl & 0x100) || !(imem_cntl & 0x200))
					break;
				imem_cntl = avago_sbus_rd(ah, sbus_addr, 0x01);
			}
			if (loops >= AVAGO_SPICO_UPLOAD_WAIT_TIMEOUT)
				avago_fail(ah, __func__, __LINE__,
					  "Timed out after %d loops while waiting for SPICO upload to complete (0x%02x 0x%08x).\n",
					  loops, avago_sbus_rd(ah, sbus_addr,
							       0x01),
					  imem_cntl);
			avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
					"Waited %d loops for pending upload to complete (0x%02x 0x%08x).\n",
					loops, avago_sbus_rd(ah, sbus_addr,
							     0x01), imem_cntl);
		}
	}
}

/** @cond INTERNAL */

static void serdes_spico_halt_fix(struct avago_hdl *ah,
			/**< [in] Pointer to Aapl_t structure. */
				  uint32_t addr,
			/**< [in] Device address number. */
				  int initial_value_0x20)
{
	if ((avago_get_lsb_rev(ah, addr) < 5)
	    && ((avago_sbus_rd(ah, addr, 0x27) & 0x3ff) == 0x142)) {
		avago_sbus_wr(ah, addr, 0x20, initial_value_0x20 | 0x3);
		if ((avago_sbus_rd(ah, addr, 0x27) & 0x3ff) == 0x047) {
			uint32_t addr_0x00;
			avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
					"Stepping processor and clearing HDW interrupt. %04x %08x %08x\n",
					avago_sbus_rd(ah, addr, 0x25),
					avago_sbus_rd(ah, addr, 0x27),
					avago_sbus_rd(ah, addr, 0x28));
			avago_sbus_wr(ah, addr, 0x20, initial_value_0x20 | 0x3);
			addr_0x00 = avago_serdes_mem_rd(ah, addr,
							AVAGO_LSB_DIRECT, 0x00);
			avago_serdes_mem_wr(ah, addr, AVAGO_LSB_DIRECT, 0x00,
					    addr_0x00 | 0x2);
		}
	}
}

/** @brief   Safely halts the SPICO processor. */
/** @details Checks that the processor did not halt on the clear interrupt */
/**          command which is right before the rti. The two commands must be */
/**          atomic.  If we stopped between them, we need to step the */
/**          processor past the RTI and then clear the interrupts again so */
/**          that any pending hardware interrupts will be seen. */
/** @details The halt/resume functions can be safely nested: */
/**          It's safe to call halt when already halted, and resume will only */
/**          resume if SPICO was running prior to the corresponding halt. */
/** @return  A SPICO run state variable to pass to avago_spico_resume(). */
/** @see avago_spico_resume(). */
int avago_spico_halt(struct avago_hdl *ah,
			/**< [in] Pointer to Aapl_t structure. */
		     uint32_t addr)
{
/**< [in] Device address number. */
	int initial_value = 0;
	int spico_running = avago_get_spico_running_flag(ah, addr);

	if (!avago_check_process(ah, addr, __func__, __LINE__, true, 2,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F) ||
	    !avago_check_ip_type(ah, addr, __func__, __LINE__, true, 4,
				 AVAGO_SPICO, AVAGO_SERDES, AVAGO_M4,
				 AVAGO_P1))
		return 0;

	if ((avago_get_ip_type(ah, addr) == AVAGO_SERDES) ||
	    (avago_get_ip_type(ah, addr) == AVAGO_M4) ||
	    (avago_get_ip_type(ah, addr) == AVAGO_P1)) {
		initial_value = avago_sbus_rmw(ah, addr, 0x20, 0x01, 0x01);
		if (0 == (initial_value & 0x01))
			serdes_spico_halt_fix(ah, addr, initial_value);

		if (avago_get_ip_type(ah, addr) == AVAGO_P1) {
			int reset_status = avago_sbus_rd(ah, addr, 0x07);
			if ((reset_status & 0x01) || !(reset_status & 0x10))
				avago_sbus_wr(ah, addr, 0x07,
					      (reset_status & ~1) | 0x10);
			initial_value |= reset_status << 16;
		}
	} else if (avago_get_ip_type(ah, addr) == AVAGO_SPICO) {
		initial_value = avago_sbus_rmw(ah, addr, 0x5, 0x01, 0x03);
	}
	avago_set_spico_running_flag(ah, addr, 0);
	return initial_value | (spico_running << 31);
}

/** @brief   Resume the SPICO run state after avago_spico_halt(). */
/** @details The halt/resume functions can be safely nested: */
/**          It's safe to call halt when already halted, and resume will only */
/**          resume if SPICO was running prior to the corresponding halt. */
/** @return  0 on success, -1 on error. */
/** @see     avago_spico_halt(). */
int avago_spico_resume(struct avago_hdl *ah,
				/**< [in] Pointer to Aapl_t structure. */
		       uint32_t addr,
				/**< [in] Device address number. */
		       int spico_run_state)
{
/**< [in] Value returned from halt function. */
	if (!avago_check_process(ah, addr, __func__, __LINE__, true, 2,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F) ||
	    !avago_check_ip_type(ah, addr, __func__, __LINE__, true, 4,
				 AVAGO_SPICO, AVAGO_SERDES, AVAGO_M4, AVAGO_P1))
		return 0;

	if (avago_get_ip_type(ah, addr) == AVAGO_P1) {
		int reset_status = (spico_run_state >> 16) & 0x7fff;
		if ((reset_status & 0x01) || !(reset_status & 0x10))
			avago_sbus_wr(ah, addr, 0x07, reset_status);
	}

	if (0 == (spico_run_state & 0x01)) {
		if ((avago_get_ip_type(ah, addr) == AVAGO_SERDES) ||
		    (avago_get_ip_type(ah, addr) == AVAGO_M4) ||
		    (avago_get_ip_type(ah, addr) == AVAGO_P1)) {
			serdes_spico_halt_fix(ah, addr, spico_run_state);
			avago_sbus_wr(ah, addr, 0x20, spico_run_state);
		}
		if (avago_get_ip_type(ah, addr) == AVAGO_SPICO) {
			avago_sbus_wr(ah, addr, 0x05, spico_run_state);
		}
		if (spico_run_state & 0x80000000)
			avago_set_spico_running_flag(ah, addr, 1);
	}
	return 0;
}

/** @endcond */
