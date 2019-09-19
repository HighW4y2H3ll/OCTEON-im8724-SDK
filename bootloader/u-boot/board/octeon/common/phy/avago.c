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

#include <common.h>
#include <malloc.h>
#include <i2c.h>
#include <phy.h>
#include "avago.h"
#include "avago_sbus.h"
#include "avago_serdes.h"
#include "avago_spico.h"
#include "avago_logging.h"
#include "avago_avsp.h"

/******************** i2c operations ****************************/

/**
 * Perform the specified SBus operation
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus address
 * @param	reg_addr	Register address
 * @param	command		Type of command, 0 = reset, 1 = write, 2 = read
 * @param	sbus_data	Data to write for SBus write command or
 *				data read from read command.
 *
 * @return	Data read from device, otherwise 0 for success, error otherwise
 */
bool avago_i2c_sbus_fn(struct avago_hdl *ah, uint32_t sbus_addr,
			   uint8_t reg_addr, uint8_t command,
			   uint32_t *sbus_data)
{
	uint8_t buffer[8] = {0};
	/* Derive device i2c address based on the chip number and base address */
	uint32_t i2c_address = ah->i2c_base_addr;
	bool rc = true;
	struct avago_addr addr_struct;

	debug("%s(0x%p, 0x%x, 0x%x, 0x%x, 0x%p)\n", __func__, ah, sbus_addr,
	      reg_addr, command, sbus_data);
	sbus_addr &= 0xffff;	/* Ignore lane */
	avago_addr_to_struct(sbus_addr, &addr_struct);
	i2c_address += addr_struct.chip;

	if (command == 0) {	/* Soft Reset */
		debug("Performing SBus soft reset\n");
		buffer[0] = RESET_SBUS_DEVICE;
		buffer[1] = 0;
		buffer[2] = (uint8_t)addr_struct.sbus;
		buffer[3] = 0;
		buffer[4] = 0;
		buffer[5] = 0;
		buffer[6] = 0;
		i2c_write(i2c_address, 0, 0, buffer, 7);
	} else if (command == 3) {	/* Hard reset */
		int retry;

		debug("Performing SBus hard reset\n");
		buffer[0] = CORE_SBUS_RESET;
		buffer[1] = 0;
		buffer[2] = 0;
		buffer[3] = 0;
		buffer[4] = 0;
		buffer[5] = 0;
		buffer[6] = 0;
		i2c_write(i2c_address, 0, 0, buffer, 7);
		for (retry = 0; retry <= AVAGO_I2C_HARD_RESET_TIMEOUT; retry++)
			if (avago_diag_sbus_rw_test(ah,
					avago_make_sbus_controller_addr(sbus_addr),
						    2))
				break;
	} else if (command == 1) {	/* Write */
		debug("Performing SBus write\n");
		buffer[0] = WRITE_SBUS_DEVICE;
		buffer[1] = reg_addr;
		buffer[2] = (uint8_t)addr_struct.sbus;
		buffer[3] = ((*sbus_data) >> 0) & 0xff;
		buffer[4] = ((*sbus_data) >> 8) & 0xff;
		buffer[5] = ((*sbus_data) >> 16) & 0xff;
		buffer[6] = ((*sbus_data) >> 24) & 0xff;
		i2c_write(i2c_address, 0, 0, buffer, 7);
	} else if (command == 2) {	/* Read */
		debug("Performing SBus read\n");
		buffer[0] = READ_SBUS_DEVICE;
		buffer[1] = reg_addr;
		buffer[2] = (uint8_t)addr_struct.sbus;
		i2c_write(i2c_address, 0, 0, buffer, 3);
		/* Read 4 bytes, Change to 5 to also read the status byte, but
		 * since we are ignoring it, don't bother reading it.
		 * Also, not reading it is somewhat faster.
		 */
		if (!i2c_read(i2c_address, 0, 0, buffer, 4)) {
			/* Construct 32-bit return value from returned byte
			 * array.
			 */
			*sbus_data = (buffer[3] << 24) | (buffer[2] << 16) ||
				     (buffer[1] << 8)  | buffer[0];
		} else {
			rc = false;
		}
	}
	debug("%s: rc: %d, sbus_data: 0x%x\n", __func__, rc,
	      sbus_data ? *sbus_data : 0);
	return rc;
}

void avago_i2c_write(struct avago_hdl *ah, int dev_addr, int length,
		     const uint8_t *buffer)
{
	char command[256];
	char *ptr;
	__attribute__((unused)) char *end;
	int i;

	i2c_write(dev_addr, 0, 0, (uint8_t *)buffer, length);

	if (ah->debug < AVAGO_DEBUG8)
		return;

	end = &command[sizeof(command) - 1];
	ptr = command;
	ptr += snprintf(command, sizeof(command), "i2c w %02x", dev_addr);
	for (i = 0; i < length; i++)
		ptr += snprintf(ptr, end - ptr, " %02x", buffer[i]);

	avago_log_printf(ah, AVAGO_DEBUG8, __func__, __LINE__,
			 "i2c write of %d bytes to addr 0x%x. Data: %s -> %d\n",
			 length, dev_addr, command, length);

}

int avago_i2c_read(struct avago_hdl *ah, int dev_addr, int length,
		   uint8_t *buffer)
{
	int rc;
	int i;
	char command[256];
	char *ptr, *end;

	debug("%s(0x%p, 0x%x, 0x%x, 0x%p)\n", __func__, ah, dev_addr,
	      length, buffer);
	rc = i2c_read(dev_addr, 0, 0, buffer, length);
	debug("%s: rc: %d\n", __func__, rc);
	if (ah->debug < AVAGO_DEBUG8)
		return rc;

	ptr = command;
	end = &command[sizeof(command) - 1];
	for (i = 0; i < length; i++)
		ptr += snprintf(ptr, end - ptr, "%02x ", buffer[i]);
	avago_log_printf(ah, AVAGO_DEBUG8, __func__, __LINE__,
			 "i2c read of %d bytes from 0x%x --> %s\n",
			 length, dev_addr, command);
	return rc;
}

bool avago_i2c_sbus(struct avago_hdl *ah, uint32_t sbus_addr,
		    uint8_t reg_addr, uint8_t command,
		    uint32_t *sbus_data)
{
	uint8_t buffer[8];
	uint32_t i2c_address = ah->i2c_base_addr;
	bool rc = true;
	struct avago_addr addr_struct;
	int retry;

	debug("%s(0x%p, 0x%x, 0x%x, 0x%x, 0x%p (0x%x)\n",
	      __func__, ah, sbus_addr, reg_addr, command, sbus_data,
	      sbus_data ? *sbus_data : 0);
	i2c_set_bus_num(ah->i2c_bus);
	sbus_addr &= 0xffff;

	avago_addr_to_struct(sbus_addr, &addr_struct);
	memset(buffer, 0, sizeof(buffer));

	if (command == 0) {		/* Soft Reset */
		buffer[0] = RESET_SBUS_DEVICE;
		buffer[2] = addr_struct.sbus;
		avago_i2c_write(ah, i2c_address, 7, buffer);

	} else if (command == 3) {	/* Hard Reset */
		buffer[0] = CORE_SBUS_RESET;
		avago_i2c_write(ah, i2c_address, 7, buffer);

		for (retry = 0; retry <= AVAGO_I2C_HARD_RESET_TIMEOUT; retry++)
			if (avago_diag_sbus_rw_test(ah,
			      avago_make_sbus_controller_addr(sbus_addr), 2))
				break;
		/* TODO wait for RW test to pass */
	} else if (command == 1) {	/* Write */
		buffer[0] = WRITE_SBUS_DEVICE;
		buffer[1] = reg_addr;
		buffer[2] = addr_struct.sbus;
		buffer[3] = (*sbus_data >>  0) & 0xff;
		buffer[4] = (*sbus_data >>  8) & 0xff;
		buffer[5] = (*sbus_data >> 16) & 0xff;
		buffer[6] = (*sbus_data >> 24) & 0xff;
		avago_i2c_write(ah, i2c_address, 7, buffer);
	} else if (command == 2) {	/* Read */
		buffer[0] = READ_SBUS_DEVICE;
		buffer[1] = reg_addr;
		buffer[2] = addr_struct.sbus;
		avago_i2c_write(ah, i2c_address, 3, buffer);
		/* Read 4 bytes,  Change to 5 to also read the status byte.
		 * Since we're ignore it, dont bother reading it, also not
		 * reading it is somewhat faster.
		 */
		if (avago_i2c_read(ah, i2c_address, 4, buffer) == 0)
			*sbus_data = (buffer[3] << 24) |
				     (buffer[2] << 16) |
				     (buffer[1] <<  8) |
				     (buffer[0] <<  0);
		else
			rc = false;
	} else {
		printf("%s: Unknown command 0x%x\n", __func__, command);
		rc = false;
	}
	debug("%s: return: %d, sbus_data: 0x%x\n", __func__, rc,
	      sbus_data ? *sbus_data : 0);
	return rc;
}
