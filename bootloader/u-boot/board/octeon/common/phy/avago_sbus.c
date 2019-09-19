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
 ** @file
 ** @brief SBus functions.
 **/

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_sbus.h"
#include "avago_logging.h"
#include "avago_serdes.h"

/*============================================================================= */
/* S B U S */
/* */
/** @brief   Execute an sbus command. */
/** @details Lowest level SBus function.  Allows the user to fully specify */
/**          all fields in the sbus command.  Generally, one should use */
/**          sbus_rd, sbus_wr and sbus_rmw instead of sbus. */
/** @return  For reads, returns read data. */
/**          For writes and reset, returns 0. */
uint32_t avago_sbus(struct avago_hdl *ah,
					/**< [in] Pointer to Aapl_t structure. */
		    uint32_t sbus_addr,
				    /**< [in] SBus slice address. */
		    uint8_t reg_addr,
				/**< [in] SBus register to read/write */
		    uint8_t command,
				/**< [in] 0=reset, 1=write, 2=read */
		    uint32_t sbus_data,
				    /**< [in] Data to write */
		    int recv_data_back)
{
/**< [in] Allows AACS protocol optimization. */
	struct avago_addr addr_struct;
	uint32_t rc;
	char sbus_cmd[256];

	sbus_addr &= 0xffff;
	avago_addr_to_struct(sbus_addr, &addr_struct);
	sbus_data &= 0xffffffff;

	if (addr_struct.chip == AVAGO_BROADCAST) {
		uint32_t data = 0;
		struct avago_addr a_struct = addr_struct;
		for (a_struct.chip = 0; a_struct.chip < ah->chips; a_struct.chip++)
			data = avago_sbus(ah, avago_struct_to_addr(&a_struct),
					  reg_addr, command, sbus_data,
					  recv_data_back);
		return data;
	}
	if (addr_struct.ring == AVAGO_BROADCAST) {
		uint32_t data = 0;
		struct avago_addr a_struct = addr_struct;
		for (a_struct.ring = 0; a_struct.ring < AVAGO_MAX_RINGS;
		     a_struct.ring++) {
			if (ah->max_sbus_addr[a_struct.chip][a_struct.ring] != 0)
				data = avago_sbus(ah,
						  avago_struct_to_addr(&a_struct),
						  reg_addr, command, sbus_data,
						  recv_data_back);
		}
		return data;
	}

	snprintf(sbus_cmd, sizeof(sbus_cmd), "sbus %x%x%02x %02x %02x 0x%08x",
		 addr_struct.chip, addr_struct.ring, addr_struct.sbus & 0xff,
		 reg_addr, command, sbus_data);

#if 0
	if (!ah->max_sbus_addr[addr_struct.chip][addr_struct.ring])
		avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
				 "SBus command (%s) destined for SBus address %s sent to ring %d which may not exist.\n",
				 sbus_cmd, avago_addr_to_str(sbus_addr),
				 addr_struct.ring);
#endif
	if (sbus_addr == 0xfe && reg_addr == 0x00 && (command & 0x3) == 1 &&
	    sbus_data == 0x81)
		recv_data_back = 1;

	rc = 0;
	if (ah->sbus_fn)
		rc = ah->sbus_fn(ah, sbus_addr, reg_addr, command,
				 &sbus_data);
	else
		avago_log_printf(ah, AVAGO_WARNING, __func__, __LINE__,
				 "SBus function function not registered.\n");
	if (!rc)
		avago_fail(ah, __func__, __LINE__,
			   "SBus function failed.\n");

	if (recv_data_back <= 1)
		avago_log_printf(ah, AVAGO_DEBUG8, __func__, __LINE__,
				 "%s -> 0x%08x\n", sbus_cmd, sbus_data);
	ah->sbus_commands++;
	return sbus_data;
}

/*============================================================================= */
/*  SBUS RD */
/* */
/** @brief  SBus read operation */
/** @return Returns the result of the read operation. */
/** */
uint32_t avago_sbus_rd(struct avago_hdl * ah,
					/**< [in] Pointer to Aapl_t structure. */
		       uint32_t sbus_addr,
				    /**< [in] SBus slice address. */
		       uint8_t reg_addr)
{
/**< [in] Register to read */
	return avago_sbus(ah, sbus_addr, reg_addr, 0x02, 0x00000000, 1);
}

/** @brief  SBus array read operation */
/** @return Returns 0 and places the results of the read operations in sbus_data[]. */
uint32_t avago_sbus_rd_array(struct avago_hdl * ah,
						/**< [in] Pointer to Aapl_t structure. */
			     uint32_t addr, /**< [in] SBus slice address. */
			     uint32_t count,/**< [in] Number of registers to read. */
			     const uint8_t *reg_addr,
					/**< [in] Register addresses to read. */
			     uint32_t sbus_data[])
{
/**< [out] Array into which to place register values. */
	uint32_t i;
	for (i = 0; i < count - 1; i++)
		sbus_data[i] =
		    avago_sbus(ah, addr, reg_addr[i], 0x02, 0x00000000, 2);
	sbus_data[i] = avago_sbus(ah, addr, reg_addr[i], 0x02, 0x00000000, 3);
	return 0;
}

/*============================================================================= */
/*  SBUS WR */
/* */
/** @brief  SBus write operation */
/** @return Returns the result of the avago_sbus() write function, which should generally be ignored. */
/** */
uint32_t avago_sbus_wr(struct avago_hdl * ah,
					/**< [in] Pointer to Aapl_t structure. */
		       uint32_t sbus_addr,
				    /**< [in] SBus slice address. */
		       uint8_t reg_addr,
				/**< [in] Register to update */
		       uint32_t sbus_data)
{
/**< [in] Data to write */
	return avago_sbus(ah, sbus_addr, reg_addr, 0x01, sbus_data, 0);
}

/*============================================================================= */
/*  SBUS WR FLUSH */
/* */
/** @brief  SBus write operation with flush */
/** @return Returns the result of the avago_sbus() write function, which should generally be ignored. */
/** */
uint32_t avago_sbus_wr_flush(struct avago_hdl * ah,
					/**< [in] Pointer to Aapl_t structure. */
			     uint32_t sbus_addr,
				    /**< [in] SBus slice address. */
			     uint8_t reg_addr,
				/**< [in] Register to update */
			     uint32_t sbus_data)
{
/**< [in] Data to write */
	return avago_sbus(ah, sbus_addr, reg_addr, 0x01, sbus_data, 1);
}

/*============================================================================= */
/*  SBUS RMW */
/* */
/** @brief  Modify some bits in an existing SBus register */
/** @return Returns the initial value of the SBus register before it was modified. */
uint32_t avago_sbus_rmw(struct avago_hdl * ah,
					/**< [in] Pointer to Aapl_t structure. */
			uint32_t sbus_addr,
				    /**< [in] SBus slice address. */
			uint8_t reg_addr,
				/**< [in] Register to update */
			uint32_t sbus_data,
				    /**< [in] Data to write */
			uint32_t mask)
{
/**< [in] Bits in existing data to modify */

	uint32_t initial_value = avago_sbus_rd(ah, sbus_addr, reg_addr);
	avago_sbus_wr(ah, sbus_addr, reg_addr,
		      (sbus_data & mask) | (initial_value & ~mask));
	return initial_value;
}

/*============================================================================= */
/* S B U S   R E S E T */
/* */
/** @brief   Perform an sbus reset action. */
/** @details NOTE: When 0xff is used as the input address, all IP types are reset (not just SerDes) */
/** */
/** @return  void */
void avago_sbus_reset(struct avago_hdl *ah,
					/**< [in] Pointer to Aapl_t structure. */
		      uint32_t sbus_addr_in,
				    /**< [in] SBus slice address. */
		      int hard)
{
	bool st;
	struct avago_addr addr_struct, start, stop, next;

	if (hard) {

#if AVAGO_ALLOW_MDIO || AVAGO_ALLOW_GPIO_MDIO
		if (avago_is_mdio_communication_method(ah)) {
			int prev_timeout;
			uint32_t reset_val;

			avago_addr_to_struct(sbus_addr_in, &addr_struct);

			reset_val = avago_mdio_rd(ah, addr_struct.chip, AVSP_DEVAD, AVAGO_MDIO_REG_RESET);

			avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
					 "Initial MDIO reset reg: 0x%0x\n",
					 reset_val);

			reset_val &= 0x1;
			reset_val |= (reset_val << 1);

			reset_val ^= 3;

			prev_timeout = ah->sbus_mdio_timeout;
			avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD,
				      AVAGO_MDIO_REG_RESET, 0x0100 | reset_val);
			avago_mdio_wr(ah, addr_struct.chip, AVSP_DEVAD,
				      AVAGO_MDIO_REG_RESET,
				      0x0100 | (reset_val ^ 3));
			ah->sbus_mdio_timeout = 10000;

			AVAGO_SUPPRESS_ERRORS_PUSH(ah);
			avago_sbus_rd(ah, avago_make_sbus_controller_addr(sbus_addr_in), 0xfe);
			AVAGO_SUPPRESS_ERRORS_POP(ah);
			ah->sbus_mdio_timeout = prev_timeout;

			avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
					 "Final MDIO reset reg: 0x%0x\n",
					 0x0100 | (reset_val ^ 3));
		} else
#endif
		if (avago_is_i2c_communication_method(ah)) {
			debug("%s: Hard reset for %s\n", __func__,
			      avago_addr_to_str(sbus_addr_in));
			ah->sbus_fn(ah, sbus_addr_in, 0, 3, 0);
		}
	}

	if (hard)
		sbus_addr_in = avago_make_serdes_broadcast_addr(sbus_addr_in);


	avago_addr_to_struct(sbus_addr_in, &addr_struct);
	for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 1, AVAGO_UNKNOWN_IP))
			continue;

		if (!hard) {
			if (avago_check_process(ah, sbus_addr, __func__,
						__LINE__, false, 1,
						AVAGO_PROCESS_B) &&
			    avago_get_ip_type(ah, sbus_addr) == AVAGO_SERDES)
				avago_sbus_wr(ah, sbus_addr, 0xff, 0x0);
			avago_sbus(ah, sbus_addr, 0x00, 0x00, 0x00, 1);
		}

		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 1, AVAGO_SERDES)
		    && avago_check_process(ah, sbus_addr, __func__, __LINE__,
					   false, 2, AVAGO_PROCESS_B,
					   AVAGO_PROCESS_F)) {
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x20000000);
		}
	}
	if (sbus_addr_in == AVAGO_BROADCAST && !hard) {
		avago_sbus(ah, avago_make_sbus_master_addr(sbus_addr_in),
			   0x00, 0x00, 0x00, 1);
		avago_sbus(ah, avago_make_sbus_controller_addr(sbus_addr_in),
			   0x00, 0x00, 0x00, 1);
	}
}

/** @brief   Performs a read-write test to the SBus Controller or SerDes */
/** @details If sbus_addr is 0 or points to an SBus controller the test will */
/**          Communicate with the SBus Controller (sbus address 0xfd), */
/**          otherwise it will run on any specified SerDes SBus address. */
/** @return  true if the test passes, false if any errors. */
bool avago_diag_sbus_rw_test(struct avago_hdl *ah,
				    /**< [in] Pointer to Aapl_t structure. */
			     uint32_t sbus_addr,
				/**< [in] SBus slice address. */
			     int cycles)
{
/**< [in] Number of write-read cycles to do. */
	struct avago_addr addr_struct;
	int result = true, x, ran, data = 0, addr, data_addr, mask;
	int orig_val;

	debug("%s(%p, %s (0x%x), %d)\n", __func__, ah,
	      avago_addr_to_str(sbus_addr), sbus_addr, cycles);
	avago_addr_to_struct(sbus_addr, &addr_struct);

	if (addr_struct.sbus != AVAGO_SBUS_CONTROLLER_ADDRESS &&
	    (!ah->ip_type[AVAGO_3D_ARRAY_ADDR(addr_struct)] ||
	     avago_get_ip_type(ah, sbus_addr) != AVAGO_SERDES)) {
		avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
				 "%s requested on SBus address %s, which doesn't support the r/w test. Testing using SBus address %s instead.\n",
				 __func__, avago_addr_to_str(sbus_addr),
				 avago_addr_to_str
				 (avago_make_sbus_controller_addr(sbus_addr)));
		sbus_addr = avago_make_sbus_controller_addr(sbus_addr);
	}

	avago_addr_to_struct(sbus_addr, &addr_struct);

	if (addr_struct.sbus == 0 ||
	    addr_struct.sbus == AVAGO_SBUS_CONTROLLER_ADDRESS) {
		addr = avago_make_sbus_controller_addr(sbus_addr);
		mask = 0xffffffff;
		data_addr = 0x13;
	} else {

		addr = sbus_addr;
		mask = 0xffff;
		data_addr = 0x26;
	}

	AVAGO_SUPPRESS_ERRORS_PUSH(ah);

	orig_val = avago_sbus_rd(ah, addr, data_addr);

	ran = 0x1678ab4a;
	ran ^= (size_t) & ran;
	for (x = 1; x <= cycles; x++) {
		ran = (ran << 1) | (((ran >> 0) ^ (ran >> 6)) & 0x1);
		ran &= mask;
		avago_sbus_wr(ah, addr, data_addr, ran);
		data = avago_sbus_rd(ah, addr, data_addr) & mask;
		if (data != ran) {
			result = false;
			break;
		}
		if (x % 1000 == 0)
			avago_log_printf(ah, AVAGO_DEBUG2, __func__,
					 __LINE__,
					 "SBus R/W %d cycles complete on address %s.\n",
					 x, avago_addr_to_str(addr));
	}

	avago_sbus_wr_flush(ah, addr, data_addr, orig_val);
	AVAGO_SUPPRESS_ERRORS_POP(ah);

	if (!result)
		avago_fail(ah, __func__, __LINE__,
			   "SBus read/write test failed on address %s in test loop %d. Expected 0x%02x, but got 0x%02x.\n",
			   avago_addr_to_str(addr), x, ran, data);
	else
		avago_log_printf(ah, AVAGO_DEBUG1, __func__, __LINE__,
				 "SBus R/W test passed on address %s after %d cycles.\n",
				 avago_addr_to_str(addr), cycles);

	return result;
}
