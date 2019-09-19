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
/* AAPL (ASIC and ASSP Programming Layer) support for AVSP */

/** Doxygen File Header */
/** @file */
/** @brief Functions common to ASSPs. */

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

static uint32_t a5410_host_addrs[] = {0x9, 0x8, 0x7, 0x6 };
static uint32_t a5410_mod_addrs[]  = { 0x5, 0x4 };
static uint32_t a5410_addrs[]      = { 0x9, 0x8, 0x7, 0x6, 0x5, 0x4 };
static const char *a5410_names[] = {
	0, 0, 0, 0, 0, "M25G0", "H10G3", "H10G2", "H10G1", "H10G0", 0, "0", "1"
};

/** @defgroup Avsp Avsp Functions */
/** @{ */

static void avsp_set_refclk_halve(struct avago_hdl *ah, uint32_t prtad,
				  bool refclk_halve)
{
	uint32_t addr = avago_make_addr3(prtad, 0, 0);
	uint32_t sbc_addr = avago_make_sbus_controller_addr(addr);

	avago_sbus_reset(ah, sbc_addr, 0);

	if (refclk_halve) {
		const char *avsp_name = avago_get_chip_name(ah, addr);

		if (!strcmp(avsp_name, "AVSP-1104"))
			avago_sbus_rmw(ah, sbc_addr, 0x0080, 0x0002, 0x0002);
		else if (!strcmp(avsp_name, "AVSP-8812"))
			avago_sbus_rmw(ah, sbc_addr, 0x0080, 0x000c, 0x000c);
		else if (!strcmp(avsp_name, "AVSP-5410"))
			avago_sbus_rmw(ah, sbc_addr, 0x0080, 0x0008, 0x0008);
		else
			avago_sbus_rmw(ah, sbc_addr, 0x0080, 0x0004, 0x0004);

		mdelay(1);
	}
}

/**
 * @brief  Returns REFCLK divider.
 *
 * @param[in]	ah	Avago handle
 * @param	prtad	Port address
 *
 * @details The REFCLK can be divided in half before passing to the SerDes.
 *          Note: This setting does not divide REFCLK for AN.
 * @return Returns 1 if the REFCLK DIV2 divider is not enabled.
 * @return Returns 2 if the REFCLK DIV2 divider is enabled.
 * @return On error, decrements ah->return_code and returns -1.
 */
int avsp_get_refclk_divisor(struct avago_hdl *ah, uint32_t prtad)
{
	int return_code = ah->return_code;
	uint32_t addr = avago_make_addr3(prtad, 0, 0);
	uint32_t sbc_addr = avago_make_sbus_controller_addr(addr);
	const char *avsp_name = avago_get_chip_name(ah, addr);
	uint32_t addr_80 = avago_sbus_rd(ah, sbc_addr, 0x80);
	int ret = 1;

	if ((!strcmp(avsp_name, "AVSP-1104")		&& (addr_80 & 0x02)) ||
	    (!strcmp(avsp_name, "AVSP-4412/8801")	&& (addr_80 & 0x04)) ||
	    (!strcmp(avsp_name, "AVSP-5410")		&& (addr_80 & 0x08)) ||
	    (!strcmp(avsp_name, "AVSP-8812")		&&
		(((addr_80 & 0x18) == 0x08) || ((addr_80 & 0x14) == 0x14)))  ||
	    (!strcmp(avsp_name, "AVSP-9104")		&& (addr_80 & 0x04)))
		ret = 2;
	return (return_code == ah->return_code) ? ret : -1;
}

/**
 * @brief   Puts the AVSP device into a known good state and uploads firmware.
 * @details Makes the device ready for additional configuration by performing
 *          soft resets, adjusting the refclk pad frequency if requested, and
 *          uploading firmware.
 *
 * @param	ah	Avago handle
 * @param	prtad	Port address of targeted device
 * @param	refclk_halve	Cut REFCLK frequency in half within the clock pad
 * @param	serdes_rom_size	Size of SerDes ROM image
 * @param	serdes_rom	Pointer to SerDes ROM image
 * @param	sbm_rom_size	Size of SBM ROM image
 * @param	sbm_rom		pointer to SBM ROM image
 * @param	swap_rom_size	Size of SDI(swap) ROM image (not used)
 * @param	swap_rom	pointer to SDI(swap) image (not used)
 *
 * @return  On success, returns 0.
 * @return  On error, decrements ah->return_code and returns -1.
 */
int avsp_upload_firmware(struct avago_hdl *ah,
			 uint32_t prtad,
			 bool refclk_halve,
			 int serdes_rom_size,
			 const int *serdes_rom,
			 int sbm_rom_size,
			 const int *sbm_rom,
			 int swap_rom_size,
			 const int *swap_rom)
{
	int return_code = ah->return_code;
	uint32_t addr = avago_make_addr3(prtad, 0, 0);
	uint32_t broadcast_addr = avago_make_serdes_broadcast_addr(addr);

	avsp_set_refclk_halve(ah, prtad, refclk_halve);
	avago_firmware_upload(ah, broadcast_addr, serdes_rom_size, serdes_rom,
			      sbm_rom_size, sbm_rom, swap_rom_size, swap_rom);

	return (return_code == ah->return_code) ? 0 : -1;
}

/**
 * Uploads firmware stored in u32 format.
 *
 * @param	ah	Avago handle
 * @param	prtad	port address to upload firmware to
 * @param	refclk_halve	Set true to cut reference clock in half
 * @param	serdes_rom_size	Size of SerDes ROM image in 32-bit words
 * @param[in]	serdes_rom	Pointer to array of uint32_t containing SerDes
 *				ROM image
 * @param	sbm_rom_size	Size of SBM ROM in 32-bit words
 * @param[in]	sbm_rom		Pointer to array of uint32_t containing SBM
 *				ROM image
 *
 * @return	0 for success, otherwise error
 */
int avsp_upload_firmware_u32(struct avago_hdl *ah, uint32_t prtad,
			     bool refclk_halve, int serdes_rom_size,
			     const uint32_t *serdes_rom, int sbm_rom_size,
			     const uint32_t *sbm_rom)
{
	int return_code = ah->return_code;
	uint32_t addr = avago_make_addr3(prtad, 0, 0);
	uint32_t broadcast_addr = avago_make_serdes_broadcast_addr(addr);
	avsp_set_refclk_halve(ah, prtad, refclk_halve);
	avago_firmware_upload_u32(ah, broadcast_addr, serdes_rom_size,
				  serdes_rom, sbm_rom_size, sbm_rom);

	return (return_code == ah->return_code) ? 0 : -1;
}

static void avsp_edit_addrs(struct avago_hdl *ah, uint32_t prtad, size_t len,
			    uint32_t * addrs)
{
	size_t i;
	(void)ah;
	for (i = 0; i < len; i++) {
		struct avago_addr addr_struct;
		avago_addr_to_struct(addrs[i], &addr_struct);
		addr_struct.chip = prtad;
		addrs[i] = avago_struct_to_addr(&addr_struct);
	}
}

#define AVSP_ARRAY_INIT(var,len,list) 	\
	((*var = list), (*len = ARRAY_SIZE(list)))

/**
 * @brief	Get SerDes addresses for an AVSP device.
 * @details	The #type field specifies whether to get all SerDes address, or
 *		just the host or mod addresses.  Note that host/mod distinctions
 *		do not make sense for all AVSP devices, and AVSP_ALL should be
 *		passed in those cases.
 *
 * @param[in]	ah	Avago handle pointer
 * @param[in]	portad	Port address of the targeted device
 * @param[in]	avsp_name	Name of AVSP device, if known
 * @param[in]	list		AVSP_ALL, AVSP_HOST, AVSP_MOD, ...
 * @param[in]	len	Number of addresses in returned #addrs.
 * @param[out]	addrs	Addresses of requested SerDes
 *
 * @return	true if known device and updates addrs and len, false otherwise.
 */
bool avago_get_addr_list(struct avago_hdl *ah,
			 uint32_t prtad,
			 const char *avsp_name,
			 enum avsp_addr_list list,
			 uint32_t * len,
			 uint32_t ** addrs)
{
	if (!avsp_name)
		avsp_name =
		    avago_get_chip_name(ah, avago_make_addr3(prtad, 0, 0));

	if (0 == strcmp(avsp_name, "AVSP-5410")) {
		if (list == AVSP_HOST)
			AVSP_ARRAY_INIT(addrs, len, a5410_host_addrs);
		else if (list == AVSP_MOD)
			AVSP_ARRAY_INIT(addrs, len, a5410_mod_addrs);
		else
			AVSP_ARRAY_INIT(addrs, len, a5410_addrs);
	} else {
		*len = 0;
		*addrs = 0;
		return false;
	}

	avsp_edit_addrs(ah, prtad, *len, *addrs);
	return true;
}

bool avsp_get_name_list(struct avago_hdl *ah,	/**< [in] Pointer to struct avago_hdl structure. */
			uint32_t prtad,		/**< [in] Port address of the targeted device. */
			const char *avsp_name,	/**< [in] Name of AVSP device, if known. */
			uint32_t * len,		/**< [out] Number of addresses in returned #addrs. */
			const char ***names)	/**< [out] Names of devices */
{


	if (!avsp_name)
		avsp_name =
		    avago_get_chip_name(ah, avago_make_addr3(prtad, 0, 0));
	if (0 == strcmp(avsp_name, "AVSP-5410")) {
		AVSP_ARRAY_INIT(names, len, a5410_names);
	} else {
		(void)avsp_name;
		*len = 0;
		*names = 0;
		return false;
	}
	return true;
}
/** @defgroup Supervisor Avsp Supervisor Functions */
/** @{ */

/** @brief   Enables supervisor monitoring of the SerDes for failure conditions. */
/** @details When failure is recognized, the hardware interr_io pin */
/**          and/or self-healing functionality is activated. */
/** @return  0 on success, -1 on failure. */
int avsp_supervisor_enable(
	struct avago_hdl *ah,       /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t chip,          /**< [in] Chip on which to enable monitoring. */
	bool enable)        /**< [in] Enable or disable monitoring. */
{
	uint32_t addr = avago_make_sbus_master_addr(avago_make_addr3(chip, 0, 0));
	uint32_t data = enable ? 0x4400 : 0x4500;
	uint32_t val  = avago_spico_int(ah, addr, 0x0026, data);
	int i;
	for (i = 0; i <= 10; i++) {
		bool enabled = !enable;
		if (0 != avsp_supervisor_status(ah, chip, &enabled) ||
		    enabled == enable)
			break;
		mdelay(10);
	}
	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "Chip %d, loop=%d, enable=%s, status=0x%x\n", chip, i,
			 avago_bool_to_str(enable), val);
	return (val & 0x10) ? -1 : 0;
}

/**
 * @brief   Retrieves whether the supervisor is enabled.
 *
 * @param	ah	Avago handle
 * @param	chip	Chip on which to enable monitoring
 * @param[out]	enable	True if monitor is running, false otherwise
 *
 * @return  0 on success, -1 on failure.
 */
int avsp_supervisor_status(
	struct avago_hdl *ah,       /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t chip,          /**< [in] Chip on which to enable monitoring. */
	bool *enabled)      /**< [out] *enabled returns true if monitor is running. */
{
	uint32_t addr = avago_make_sbus_master_addr(avago_make_addr3(chip,0,0));
	uint32_t data = 0x3400;
	uint32_t val  = avago_spico_int(ah, addr, 0x0026, data);
	*enabled = 0 != (val & 0x1000);
	avago_log_printf(ah, AVAGO_DEBUG6, __func__, __LINE__,
			 "Chip %d, enabled=%s, status=0x%x\n", chip,
			 avago_bool_to_str(*enabled), val);
	return (val & 0x10) ? -1 : 0;
}

/**
 * @brief    Clears the interr_io (interr_l) supervisor flag.
 * @details  If the conditions for activating this flag continue to exist, and
 *           monitoring is enabled, the pin will be immediately re-activated.
 * @details  Note: The interr_io supervisor flag propagates to the hardware
 *           pin via the core port and mdio state machine polling.  Core
 *           port interrupts must be enabled for this propagation to occur.
 * @return   0 on success, -1 on failure.
 */
int avsp_supervisor_clear_interr_io(
	struct avago_hdl *ah,   /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t    chip)   /**< [in] Chip upon which to operate. */
{
	uint32_t addr = avago_make_sbus_master_addr(avago_make_addr3(chip, 0, 0));
	uint32_t data = 0x4700;
	uint32_t val  = avago_spico_int(ah, addr, 0x0026, data);
	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "SBus %s, val=0x%x\n", avago_addr_to_str(addr), val);
	return (val & 0x10) ? -1 : 0;
}

static int avsp_get_serdes_count(struct avago_hdl *ah, uint32_t chip)
{
	int serdes_count;

	serdes_count = 10;	/* 5410 */

	return serdes_count;
}

/** @return 0 on success, -1 on failure. */
static int write_state_memory(
	struct avago_hdl *ah,   /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t addr,      /**< [in] SBM address. */
	uint32_t select,    /**< [in] Supervisor SerDes selection. */
	int  mem,       /**< [in] Which memory location to write. */
	int  data)      /**< [in] Data to write. */
{
	int return_code = ah->return_code;
	avago_spico_int(ah, addr, 0x25, 0x00 | (select & 0x3f));
	avago_spico_int(ah, addr, 0x25, 0x40 | (mem    & 0x3f));
	avago_spico_int(ah, addr, 0x25, 0x80 | (data   & 0x7f));
	return (return_code == ah->return_code) ? 0 : -1;
}

/**
 * @brief   Initializes an AVSP supervisor structure to the given settings.
 *
 * @param[in]	ah	Avago handle pointer
 * @param	mode	Select a configuration mode
 * @param	squelch_tx	Squelch tx while rx data is bad
 * @param	reuse_ical	Reuse initial ical values during signal recovery
 * @param	adaptive	Run continuous adaptive tuning during normal
 *				operation
 * @param[out]	config		Configuration to initialize
 *
 * @details Provides configuration options for how to initialize self-healing
 *          features, including options for what to do on initial link bring
 *          up, what to do on future link down events, and some independent
 *          options.
 * @return true if mode is a valid value, false otherwise.
 */
bool avsp_supervisor_config_init(
	struct avago_hdl *ah,                       /**< [in] Pointer to struct avago_hdl structure. */
	enum avsp_supervisor_mode mode,        /**< [in] Select a configuration mode. */
	bool squelch_tx,                    /**< [in] Squelch tx while rx data is bad. */
	bool reuse_ical,                    /**< [in] Reuse initial iCal values during signal recovery. */
	bool adaptive,                      /**< [in] Run continuous adaptive tuning during normal operation. */
	struct avsp_supervisor_config *config)   /**< [out] Configuration to initialize. */
{
	memset(config, 0, sizeof(*config));

	switch (mode) {
	case AVSP_SUPERVISOR_MODE_TUNE_IF_SIGNAL:
		config->assp_control_0 = 0x02;  /* Tune before flock */
		break;
	case AVSP_SUPERVISOR_MODE_TUNE_IF_LOCKED_SIGNAL:
		config->assp_control_0 = 0x00;  /* Tune after flock */
		break;
	case AVSP_SUPERVISOR_MODE_NO_TUNE:
		config->assp_control_0 = 0x20;  /* Skip all tuning. */
		break;
	default:
		avago_fail(ah, __func__,__LINE__,"Invalid mode value: %d\n",mode);
		return false;
	}
	if (squelch_tx)
		config->assp_control_0 |= 0x04; /* Enable TX squelch. */
	if (reuse_ical)
		config->assp_control_0 |= 0x10; /* Enable reuse iCal. */
	if (adaptive)
		config->assp_control_0 |= 0x40; /* Enable adaptive. */

	config->assp_control_1 = 0x24;
	config->assp_control_2 = 0x00;
	config->assp_control_3 = 0x00;

	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "control = %x, %x, %x, %x\n",
			 config->assp_control_0, config->assp_control_1,
			 config->assp_control_2, config->assp_control_3);
	return true;
}

/**
 * @brief   Constructs an AVSP supervisor structure with the given settings.
 * @details Provides configuration options for how to initialize self-healing
 *          features, including options for what to do on initial link bring
 *          up, what to do on future link down events, and some independent
 *          options.
 *
 * @param	ah	Avago handle
 * @param	mode	Select a configuration mode
 * @param	squelch_tx	Squelch tx while rx data is bad
 * @param	reuse_ical	Reuse initial ical values during signal recovery.
 * @param	adaptive	Run continuous adaptive tuning during normal
 *				operation.
 *
 * @return  Allocated structure.  Call avago_supervisor_config_destruct() to
 *          free it.
 * @see     avsp_supervisor_config_destruct(), avsp_supervisor_config_init().
 */
struct avsp_supervisor_config *
avsp_supervisor_config_construct(struct avago_hdl *ah,
				 enum avsp_supervisor_mode mode,
				 bool squelch_tx,
				 bool reuse_ical,
				 bool adaptive)
{
	size_t bytes = sizeof(struct avsp_supervisor_config);
	struct avsp_supervisor_config *config =
				(struct avsp_supervisor_config *)malloc(bytes);

	if (config == 0)
		return NULL;

	avago_log_printf(ah, AVAGO_DEBUG3, __func__, __LINE__,
			 "Construct supervisor state %d\n", mode);

	if (avsp_supervisor_config_init(ah, mode, squelch_tx, reuse_ical,
					adaptive, config))
		return config;

	avsp_supervisor_config_destruct(ah, config);
	return NULL;
}

/** @brief   Releases the resources associated with #config. */
/** @see     avsp_supervisor_config_construct(). */
void avsp_supervisor_config_destruct(
	struct avago_hdl *ah,                       /**< [in] Pointer to struct avago_hdl structure. */
	struct avsp_supervisor_config *config)   /**< [in] Supervisor struct to destroy. */
{
	free(config);
}

/** @brief   Sets the supervisor configuration. */
/** @return  On success, returns 0. */
/** @return  On error, decrements ah->return_code and returns -1. */
int avsp_supervisor_set_config(
	struct avago_hdl *ah,                       /**< [in] Pointer to struct avago_hdl structure. */
	uint32_t prtad,                         /**< [in] Port address of the device. */
	struct avsp_supervisor_config *config)   /**< [in] Values to write to device. */
{
	int return_code = ah->return_code;
	uint32_t addr = avago_make_sbus_master_addr(avago_make_addr3(prtad, 0, 0));

	config->serdes_count = avsp_get_serdes_count(ah, prtad);

	avsp_supervisor_enable(ah, prtad, false);
	avago_spico_int(ah, addr, 0x26, 0x6200 | (config->assp_control_0 & 0xff));
	avago_spico_int(ah, addr, 0x26, 0x6300 | (config->assp_control_1 & 0xff));
	avago_spico_int(ah, addr, 0x26, 0x7200 | (config->assp_control_2 & 0xff));
	avago_spico_int(ah, addr, 0x26, 0x7300 | (config->assp_control_3 & 0xff));

	if ((config->assp_control_0 & 1) == 0)
		avsp_supervisor_enable(ah, prtad, true);

	avago_log_printf(ah, AVAGO_DEBUG5, __func__, __LINE__,
			 "Chip %d, Config = (0x%02x, 0x%02x, 0x%02x, 0x%02x)\n",
			 prtad, config->assp_control_0, config->assp_control_1,
			 config->assp_control_2, config->assp_control_3);
	return (return_code == ah->return_code) ? 0 : -1;
}

/**
 * @brief   Tells the user is self healing is enabled.
 * @return  TRUE if it is running, FALSE otherwise
 */
bool avsp_get_self_healing(struct avago_hdl *ah, uint32_t prtad)
{
	uint32_t addr = avago_make_sbus_master_addr(avago_make_addr3(prtad, 0, 0));
	if (avago_spico_int(ah, addr, 0x26, 0x3500) & 0x1fff)
		return true;
	return false;
}

/** @brief   Allows the user to enable/disable self healing */
void avsp_set_self_healing(struct avago_hdl *ah, uint32_t prtad,
			   bool self_heal_enable)
{
	uint32_t addr = avago_make_sbus_master_addr(avago_make_addr3(prtad, 0, 0));
	avago_spico_int(ah, addr, 0x26, self_heal_enable ? 0x4400 : 0x4500);
}

/** @brief   Sets the signal OK threshold value for the device. */
/** @details Updates the state configuration in the supervisor, which applies */
/**          the updated values to the SerDes when it starts or heals a link. */
/** @return  0 on succes, -1 on failure. */
int avsp_supervisor_set_signal_ok(
	struct avago_hdl *ah,       /**< [in] Pointer to Aapl_t structure. */
	uint32_t prtad,         /**< [in] Port address of the device. */
	uint32_t select,        /**< [in] Supervisor SerDes selection. 0x2f=all. */
	int threshold)      /**< [in] Threshold for signal OK (0-15). */
{
	uint32_t addr = avago_make_sbus_master_addr(avago_make_addr3(prtad, 0, 0));
	int thresh_reg = 0x2c;
	return write_state_memory(ah, addr, select, thresh_reg, threshold & 0xf);
}
