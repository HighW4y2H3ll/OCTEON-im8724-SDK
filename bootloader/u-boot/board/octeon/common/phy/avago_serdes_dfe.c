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
/* AAPL Revision: 2.3.1-beta                                        */

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_serdes.h"
#include "avago_sbus.h"
#include "avago_logging.h"
#include "avago_spico.h"

/** Doxygen File Header */
/** @file */
/** @brief Functions related to the DFE. */

#define DFE_ROW_COL(row,col) ((row << 8) | (col << 12))

static int serdes_get_dfe_status(struct avago_hdl *ah, uint32_t addr)
{
	uint32_t interrupt_code = 0x126;
	uint32_t row = 11;
	uint32_t column = 0;

	switch (avago_get_sdrev(ah, addr)) {
	case AVAGO_SDREV_P1:
		interrupt_code = 0x2026;
		row = 1;
		column = 2;
		break;
	default:
		interrupt_code = 0x126;
		row = 11;
		column = 0;
	}
	return avago_spico_int(ah, addr, interrupt_code,
			       DFE_ROW_COL(row, column));
}

#define AVAGO_DFE_ACTIVE           0x37
#define AVAGO_DFE_ADAPTIVE_ENABLED 0x40

/**
 * @brief   Check if DFE is running.
 *
 * @param	ah	Avago handle
 * @param	addr	Address of dfe to check
 *
 * @return  Return true if dfe is running, false otherwise.
 */
bool avago_serdes_dfe_running(struct avago_hdl *ah, uint32_t addr)
{
	int dfe_status = serdes_get_dfe_status(ah, addr);
	return (dfe_status & AVAGO_DFE_ACTIVE) != 0;
}

/**
 * @brief   Wait until any running DFE completes.
 * @details Call avago_serdes_dfe_pause() rather than this function.
 *
 * @param	ah	Avago handle
 * @param	addr	Address of SerDes
 *
 * @return  Return 1 if dfe has stopped, 0 if dfe is still running.
 */
int avago_serdes_dfe_wait(struct avago_hdl *ah, uint32_t addr)
{
	int dfe_status = serdes_get_dfe_status(ah, addr);
	int loop_count = 0;
	int dfe_running = 0;
	int old_timeout = ah->serdes_int_timeout;
	enum avago_log_type log_level = AVAGO_DEBUG3;
	if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false,
				     1, 0x1054))
		ah->serdes_int_timeout = ah->serdes_int_timeout * 5 / 2;
	avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
			 "SBus %s, dfe_status 0x%x, Waiting for tuning to stop.\n",
			 avago_addr_to_str(addr), dfe_status);
	while ((dfe_status & AVAGO_DFE_ACTIVE) &&
	       loop_count++ < ah->serdes_int_timeout) {
		if (!(dfe_status & AVAGO_DFE_ADAPTIVE_ENABLED) &&
		    !avago_check_firmware_rev(ah, addr, __func__, __LINE__,
					      false, 1, 0x1044) &&
		    ((dfe_status & 0x30) && !(dfe_status & 0x03)) ) {
			avago_serdes_mem_rmw(ah, addr, AVAGO_DMEM, 0x200,
					     0x40, 0x40);
			avago_log_printf(ah, AVAGO_WARNING, __func__, __LINE__,
					 "SBus %s, Running DFE Workaround\n",
					 avago_addr_to_str(addr));
		}
		mdelay(40);
		if (ah->debug >= 4 && (loop_count % 100) == 0) {
			char buf[60], *ptr = buf;
			int i;
			for (i = 0; ptr-buf < (int)(sizeof(buf))-10; i++) {
				uint32_t pc = avago_sbus_rd(ah, addr, 0x25);
				ptr += sprintf(ptr, "%x, ", pc);
			}
			avago_log_printf(ah, AVAGO_DEBUG4, __func__, __LINE__,
					 "SBus %s, dfe_status 0x%x, pc samples=%s\n",
					 avago_addr_to_str(addr), dfe_status,
					 buf);
		}
		dfe_status = serdes_get_dfe_status(ah, addr);
	}
	dfe_running = dfe_status & AVAGO_DFE_ACTIVE;
	if (dfe_running)
		log_level = AVAGO_WARNING;
	ah->serdes_int_timeout = old_timeout;
	avago_log_printf(ah, log_level, __func__, __LINE__,
			 "SBus %s, dfe_status 0x%x, DFE tuning %s, loops=%d.\n",
			 avago_addr_to_str(addr), dfe_status,
			 dfe_running ? "stop timed out" : "stopped",
			 loop_count);
	return !dfe_running;
}

/**
 * @brief   Block new DFE actions from starting, and
 *          then wait on any in progress.
 *
 * @param	ah	Avago handle
 * @param	addr	Address of SerDes to pause
 * @param[out]	rr_enable	Return if RR was enabled
 *
 * @details Each SerDes may participate in round-robin (rr) pCal or continuous
 *          adaptive pCal, both of which can start DFE at any time. Since a
 *          running DFE interferes with eye and escope measurement,
 *          avago_serdes_pause_dfe() is provided to block any new pCal from
 *          starting and then wait for any in-progress tuning to finish.
 *          <br>
 *          Note on firmware support:
 *          0x1049 and newer: Pause and resume are fully supported, pause and
 *            resume are idempotent, and rr_enabled is unused and may be NULL.
 *          In pre-0x1049 releases, adaptive pCal is stopped and the user must
 *              restart it themselves.
 *          In release 0x1048, the rr_enabled field must be used to properly
 *              resume the rr state.  In pre-0x1048, rr is not supported.
 * @return  On success, returns 0.
 *          On error, decrements aapl->return_code and returns -1.
 *          If the wait on in-progress DFE times out, a -1 is returned without
 *          aapl->return_code being decremented.  In this case, call
 *          avago_serdes_dfe_pause() again to wait an additional period of time
 *          for DFE to complete.
 */
int avago_serdes_dfe_pause(
	struct avago_hdl *ah,       /**< [in] Pointer to Aapl_t structure. */
	uint32_t addr,          /**< [in] Device address number. */
	uint32_t *rr_enabled)   /**< [out] Return if RR was enabled (0x1048 only). */
{
	if (rr_enabled)
		*rr_enabled = false;
	if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false,
				     1, 0x1049)) {
		if (!avago_spico_int_check(ah, __func__, __LINE__, addr,
					   0x0a, 0x00))
			return -1;

		return avago_serdes_dfe_wait(ah, addr) == 1 ? 0 : -1;
	}


	if (avago_check_firmware_rev(ah,addr, __func__, __LINE__, false,
				     1, 0x1048)) {
		if (rr_enabled)
			*rr_enabled = avago_spico_int(ah, addr, 0x126, 0x5400);
		if (!rr_enabled || *rr_enabled)
			if (!avago_spico_int_check(ah, __func__, __LINE__,
						   addr, 0x0a, 0x08))
				return -1;
	}

	if (serdes_get_dfe_status(ah, addr) & AVAGO_DFE_ADAPTIVE_ENABLED) {
		struct avago_serdes_dfe_tune control;
		avago_serdes_tune_init(ah, &control);
		control.tune_mode = AVAGO_DFE_STOP_ADAPTIVE;
		avago_serdes_tune(ah, addr, &control);
	}

	return avago_serdes_dfe_wait(ah, addr) == 1 ? 0 : -1;
}

/**
 * @brief   Resumes the previously paused DFE run state.
 *
 * @param	ah	Avago Handle
 * @param	addr	SerDes address number
 * @param	rr_enable	Enable rr.
 *
 * @details Resumes the DFE execution state most recently saved by the
 *          avago_serdes_dfe_pause operation. In particular,
 *          if DFE adaptive tuning was running, it resumes running. And if
 *          round robin pCal participation was enabled, it is re-enabled.
 *          Note that repeating a resume operation has no additional effect,
 *          but repeating a pause operation will clear the resume state.
 *          Note for 0x1048 firmware, RR participation is restored.
 *          For pre-0x1049 firmware, adaptive pCal must be restarted
 *          by the caller, if desired.
 * @return  On success, returns 0.
 *          On error, decrements aapl->return_code and returns -1.
 */
int avago_serdes_dfe_resume(struct avago_hdl *ah, uint32_t addr,
			    uint32_t rr_enable)     /**< [in] Enable RR if TRUE (0x1048 release only). */
{
	if (avago_check_firmware_rev(ah, addr, __func__, __LINE__, false,
				     1, 0x1049)) {
		return avago_spico_int_check(ah, __func__, __LINE__, addr,
					     0x0a, 0x0f) ? 0 : -1;
	}

	if (rr_enable && avago_check_firmware_rev(ah,addr, __func__, __LINE__,
						  false, 1, 0x1048))
		if (!avago_spico_int_check(ah, __func__, __LINE__, addr,
					   0x0a, 0x0a))
			return -1;

	return 0;
}

