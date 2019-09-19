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

/* AAPL (ASIC and ASSP Programming Layer) support for talking with SPICO
 * processors in SBus master and SerDes slices.
 */

/** Doxygen File Header
 * * @file
 ** @brief Declarations for SPICO processor functions.
 **/

#ifndef __AVAGO_SPICO_H__
#define __AVAGO_SPICO_H__

#define AVAGO_SPICO_HALT 2

enum avago_spico_state {
	AVAGO_SPICO_RESET=0,
	AVAGO_SPICO_RUNNING=1,
	AVAGO_SPICO_PAUSE=2,
	AVAGO_SPICO_ERROR=3
};

/** @brief SPICO processor status values */
struct avago_spico_status {
	uint32_t enabled;             /** Indicates if processor is enabled and runnning */
	uint32_t pc;                  /** Program Counter value */
	uint32_t revision;            /** Revision of firmware */
	uint32_t build;               /** Build ID of firmware */
	enum avago_serdes_spico_clk clk; /** Clock Processor is running on */
	enum avago_spico_state state; /** State the Processor is in */
};

int  avago_spico_status(struct avago_hdl *ah, uint32_t sbus_addr, bool no_cache,
			struct avago_spico_status *st);
int avago_spico_running(struct avago_hdl *ah, uint32_t sbus_addr);
int  avago_spico_reset(struct avago_hdl *ah, uint32_t sbus_addr);
uint32_t avago_spico_get_pc(struct avago_hdl *ah, uint32_t addr);
uint32_t avago_spico_get_state(struct avago_hdl *ah, uint32_t addr);


int avago_spico_upload_swap_image(struct avago_hdl *ah, uint32_t sbus_addr,
				  int words, const int rom[]);
int avago_spico_upload(struct avago_hdl *ah, uint32_t sbus_addr, bool ram_bist,
		       int words, const int rom[]);
int avago_firmware_upload(struct avago_hdl *ah, uint32_t addr,
			  int serdes_rom_size, const int *serdes_rom,
			  int sbm_rom_size,    const int *sbm_rom,
			  int sdi_rom_size,    const int *sdi_rom);

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
			      const uint32_t *sbm_rom);

void avago_spico_wait_for_upload(struct avago_hdl *ah, uint32_t sbus_addr);
void avago_twi_wait_for_complete(struct avago_hdl *ah, uint32_t sbus_addr);

uint32_t avago_spico_int(struct avago_hdl *ah, uint32_t sbus_addr, int int_num,
			 int param);

uint32_t avago_serdes_spico_int_le_fn(struct avago_hdl *ah, uint32_t sbus_addr,
				      int int_num, int param);

uint32_t avago_serdes_spico_int_sbus_fn(struct avago_hdl *ah, uint32_t sbus_addr,
					int int_num, int param);

bool avago_spico_int_check(struct avago_hdl *ah, const char *caller, int line,
			   uint32_t addr, int int_num, int param);

uint32_t avago_spico_broadcast_int(struct avago_hdl *ah, int int_num,
				   int param, int args, ...);

uint32_t avago_spico_broadcast_int_w_mask(struct avago_hdl *ah,
					  uint32_t addr_mask, int int_num,
					  int param, int args, ...);

enum avago_spico_int_flags {
	AVAGO_SPICO_INT_ALL=0,
	AVAGO_SPICO_INT_FIRST=1,
	AVAGO_SPICO_INT_NOT_FIRST=2
};

struct avago_spico_int {
	int                     interrupt;
	int                     param;
	int                     ret;
	enum avago_spico_int_flags flags;
};

int  avago_spico_int_array(struct avago_hdl *ah, uint32_t sbus_addr,
			   int num_elements, struct avago_spico_int *interrupts);

uint32_t avago_firmware_get_rev(     struct avago_hdl *ah, uint32_t sbus_addr);

uint32_t avago_firmware_get_build_id(struct avago_hdl *ah, uint32_t sbus_addr);

uint32_t avago_spico_crc(            struct avago_hdl *ah, uint32_t sbus_addr);

int avago_spico_ram_bist( struct avago_hdl *ah, uint32_t sbus_addr);

void avago_crc_one_byte(int *crc_ptr, int value);

int avago_crc_rom(int *memory, int length);

int avago_spico_halt(struct avago_hdl *ah, uint32_t addr);
int avago_spico_resume(struct avago_hdl *ah, uint32_t addr, int spico_run_state);

#endif /* __AVAGO_SPICO_H__ */
