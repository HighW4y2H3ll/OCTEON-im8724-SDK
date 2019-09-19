#include <cvmx.h>
#include <cvmx-twsi.h>
#include <cvmx-core.h>
#include <octeon-model.h>

#define OCTEON_I2C_TIMEOUT	50	/* 50ms */

/* Controller command and status bits */
#define TWSI_CTL_CE		0x80
#define TWSI_CTL_ENAB		0x40
#define TWSI_CTL_STA		0x20
#define TWSI_CTL_STP		0x10
#define TWSI_CTL_IFLG		0x08
#define TWSI_CTL_AAK		0x04

/* Some status values */
#define STAT_START		0x08
#define STAT_RSTART		0x10
#define STAT_TXADDR_ACK		0x18
#define STAT_TXDATA_ACK		0x28
#define STAT_RXADDR_ACK		0x40
#define STAT_RXADDR_NACK	0x48
#define STAT_RXDATA_ACK		0x50
#define STAT_IDLE		0xF8

#ifdef CONFIG_OCTEON_OCX
# define node_bus_to_i2c_bus(node, bus)	((node << 1) | (bus))
# define i2c_bus_to_node(i2c_bus)	(((i2c_bus) >> 1) & 0x3)
#else
# define node_bus_to_i2c_bus(node, bus)	(bus)
# define i2c_bus_to_node(i2c_bus)	(0)
#endif

//#define DEBUG		1

#ifdef DEBUG
#define debug   printf
#else
#define debug(...)
#endif

/**
 * Gets the i2c bus number without the node number.
 *
 * @param bus_num	Bus number with optional node number encoded in bit 1.
 * @return bus number with the node removed.
 */
static inline int octeon_i2c_extract_bus_num(int bus_num)
{
#ifdef CONFIG_OCTEON_OCX
	return bus_num & 1;
#else
	return bus_num;
#endif
}
/**
 * Reads bytes from memory and copies to eeprom.
 * Only supports address size of 2 (16 bit internal address.)
 *
 * We can only write two bytes at a time to the eeprom, so in some cases
 * we need to to a read/modify/write.
 *
 * Note: can't do single byte write to last address in EEPROM
 *
 * @param bus    i2c adapter
 * @param chip   chip address
 * @param addr   internal address
 * @param alen   address length, must be 0, 1, or 2
 * @param buffer memory buffer pointer
 * @param len    number of bytes to write
 *
 * @return 0 on Success
 *         1 on Failure
 */
int octeon_i2c_write(unsigned int bus, uint8_t chip, uint addr,
			    int alen, uint8_t *buffer, int len);

/**
 * Write to the mio_twsx_sw_twsi register
 *
 * @param i2c_bus - TWSI bus number to write to
 * @param sw_twsi - register value to write.  Note that the v and r fields
 *                  are automatically set.
 */
static void octeon_i2c_write_sw(int i2c_bus, cvmx_mio_tws_sw_twsi_t sw_twsi)
{
	int node = i2c_bus_to_node(i2c_bus);

	i2c_bus = octeon_i2c_extract_bus_num(i2c_bus);

	sw_twsi.s.v = 1;
	sw_twsi.s.r = 0;
	cvmx_write_csr_node(node,
			    CVMX_MIO_TWSX_SW_TWSI(i2c_bus), sw_twsi.u64);
	do {
		sw_twsi.u64 = cvmx_read_csr_node(node,
						 CVMX_MIO_TWSX_SW_TWSI(i2c_bus));
	} while (sw_twsi.s.v != 0);
}

/**
 * Read from the mio_twsx_sw_twsi register
 *
 * @param i2c_bus - bus number to read from
 * @param sw_twsi - used to specifyt the control register to read by the
 *                  op and eop_ia fields.
 *
 * @return mio_twsx_sw_twsi register value with the specified control register
 *                          in the d field.
 */
static cvmx_mio_tws_sw_twsi_t octeon_i2c_read_sw(int i2c_bus,
						 cvmx_mio_tws_sw_twsi_t sw_twsi)
{
	int node = i2c_bus_to_node(i2c_bus);
	sw_twsi.s.v = 0;	/* set as not valid */

	i2c_bus = octeon_i2c_extract_bus_num(i2c_bus);
	sw_twsi.s.v = 1;
	sw_twsi.s.r = 1;
	cvmx_write_csr_node(node,
			    CVMX_MIO_TWSX_SW_TWSI(i2c_bus), sw_twsi.u64);
	do {
		sw_twsi.u64 = cvmx_read_csr_node(node,
						 CVMX_MIO_TWSX_SW_TWSI(i2c_bus));
	} while (sw_twsi.s.v != 0);

	return sw_twsi;
}

/**
 * Writes to the mio_tws_int register
 *
 * @param i2c_bus - bus number to write to
 * @param int_reg - register value to write
 */
static void octeon_i2c_write_int(int i2c_bus, cvmx_mio_tws_int_t int_reg)
{
	int node = i2c_bus_to_node(i2c_bus);
	i2c_bus = octeon_i2c_extract_bus_num(i2c_bus);
	cvmx_write_csr_node(node,
			    CVMX_MIO_TWSX_INT(i2c_bus), int_reg.u64);
	cvmx_read_csr_node(node,
			   CVMX_MIO_TWSX_INT(i2c_bus));
}

/**
 * Test the twsi control register interrupt flag bit used when the HLC is
 * disabled.
 *
 * @param bus - bus number to read from
 *
 * @return 1 if IFLG is set, 0 if clear
 */
static int octeon_i2c_test_iflg(int bus)
{
	cvmx_mio_tws_sw_twsi_t sw_twsi;

	sw_twsi.u64 = 0;
	sw_twsi.s.op = 6; 	/* EOP_IA */
	sw_twsi.s.eop_ia = TWSI_CTL;
	sw_twsi = octeon_i2c_read_sw(bus, sw_twsi);

	return sw_twsi.s.d & 0x08 ? 1 : 0;
}

/**
 * Reads the twsi status register
 *
 * @param bus - bus number to read from
 *
 * @return twsi status register value
 */
static uint8_t octeon_i2c_read_status(int bus)
{
	cvmx_mio_tws_sw_twsi_t sw_twsi;

	sw_twsi.u64 = 0;
	sw_twsi.s.op = 6;	/* EOP IA */
	sw_twsi.s.eop_ia = TWSI_STAT;
	sw_twsi = octeon_i2c_read_sw(bus, sw_twsi);
	return sw_twsi.s.d;
}

/**
 * Writes to the twsi data register
 *
 * @param bus - bus number to write to
 * @param value - data value to write
 */
static void octeon_i2c_write_data(int bus, uint32_t value)
{
	cvmx_mio_tws_sw_twsi_t sw_twsi;
	sw_twsi.u64 = 0;
	sw_twsi.s.op = 6;	/* EOP IA */
	sw_twsi.s.eop_ia = TWSI_DATA;
	sw_twsi.s.d = value;
	octeon_i2c_write_sw(bus, sw_twsi);
}

/**
 * Reads the TWSI data register
 *
 * @param bus - bus number to read from
 *
 * @return value of the TWSI data register
 */
static uint32_t octeon_i2c_read_data(int bus)
{
	cvmx_mio_tws_sw_twsi_t sw_twsi;
	sw_twsi.u64 = 0;
	sw_twsi.s.op = 6;	/* EOP IA */
	sw_twsi.s.eop_ia = TWSI_DATA;
	sw_twsi = octeon_i2c_read_sw(bus, sw_twsi);
	return sw_twsi.s.d;
}

/**
 * Writes to the twsi control register
 *
 * @param bus - bus number to write to
 * @param value - value to write to the control register
 */
static void octeon_i2c_write_ctl(int bus, uint32_t value)
{
	cvmx_mio_tws_sw_twsi_t sw_twsi;
	sw_twsi.u64 = 0;
	sw_twsi.s.op = 6;		/* EOP IA */
	sw_twsi.s.eop_ia = TWSI_CTL;	/* TWSI CTL */
	sw_twsi.s.d = value;
	octeon_i2c_write_sw(bus, sw_twsi);
}

/**
 * Manually clear the I2C bus and send a stop
 *
 * @param bus - bus number to unblock
 */
static void octeon_i2c_unblock(int bus)
{
	int i;
	cvmx_mio_twsx_int_t int_reg;

	debug("%s: Unblocking bus %d\n", __func__, bus);
	int_reg.u64 = 0;
	for (i = 0; i < 9; i++) {
		int_reg.s.scl_ovr = 0;
		octeon_i2c_write_int(bus, int_reg);
		cvmx_wait(5);
		int_reg.s.scl_ovr = 1;
		octeon_i2c_write_int(bus, int_reg);
		cvmx_wait(5);
	}
	int_reg.s.sda_ovr = 1;
	octeon_i2c_write_int(bus, int_reg);
	int_reg.s.scl_ovr = 0;
	cvmx_wait(5);
	octeon_i2c_write_int(bus, int_reg);
	int_reg.u64 = 0;
	cvmx_wait(5);
	octeon_i2c_write_int(bus, int_reg);
	cvmx_wait(5);
}

/**
 * Wait for the iflg to be set
 *
 * @param bus - bus number to wait for
 *
 * @returns 0 on success, -1 on error
 */
static int octeon_i2c_wait(int bus)
{
	int timeout = OCTEON_I2C_TIMEOUT * 1000;

	while (!octeon_i2c_test_iflg(bus) && timeout-- > 0)
		cvmx_wait(1);

	if (timeout <= 0) {
		printf("%s: Timed out on bus %d\n", __func__, bus);
		return -1;
	}

	return 0;
}

/**
 * Returns true if the status code indicates arbitration was lost
 *
 * @param code - twsi status register value
 *
 * @return 1 if arbitration was lost, 0 if not lost
 */
static bool octeon_i2c_lost_arb(uint8_t code)
{
	switch (code) {
	/* Arbitration lost in address or data byte */
	case 0x38:
	/*
	 * Arbitration lost in address as master, slave address +
	 * write bit received, ACK transmitted.
	 */
	case 0x68:
	/*
	 * Arbitration lost in address as master, general call address
	 * received, ACK transmitted.
	 */
	case 0x78:
	/*
	 * Arbitration lost in address as master, slave address + read
	 * bit received, ACK transmitted.
	 */
	case 0xb0:
		return true;
	default:
		return false;
	}
}

/**
 * send START to the bus.
 * @param bus - bus number to send start on
 *
 * @returns 0 on success, -1 on error
 */
static int octeon_i2c_start(int bus)
{
	int result;
	uint8_t stat;

	debug("%s: Starting bus %d\n", __func__, bus);
	octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB | TWSI_CTL_STA);

	result = octeon_i2c_wait(bus);
	if (result) {
		stat = octeon_i2c_read_status(bus);
		if (stat == STAT_IDLE) {
			/*
			 * Controller refused to send start flag May
			 * be a client is holding SDA low - let's try
			 * to free it.
			 */
			debug("%s: Unblocking bus %d\n", __func__, bus);
			octeon_i2c_unblock(bus);

			octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB | TWSI_CTL_STA);
			result = octeon_i2c_wait(bus);
		}
		if (result) {
			debug("%s: timeout waiting for IDLE state\n", __func__);
			return result;
		}
	}

	stat = octeon_i2c_read_status(bus);
	if ((stat != STAT_START) && (stat != STAT_RSTART)) {
		debug("%s: Bad status on bus %d (0x%x)\n", __func__, bus, stat);
		return -1;
	}
	return 0;
}

/**
 * send STOP to the bus.
 * @param bus - bus number to send stop on
 *
 * @return 0 on success, -1 on failure
 */
static int octeon_i2c_bus_stop(int bus)
{
	uint8_t stat;

	debug("%s: Stopping bus %d\n", __func__, bus);
	octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB | TWSI_CTL_STP);
	stat = octeon_i2c_read_status(bus);
	if (stat != STAT_IDLE) {
		debug("%s: bad status on bus %d (0x%x)\n", __func__, bus, stat);
		return -1;
	}
	return 0;
}

/**
 * Send data to the bus
 *
 * @param bus	 bus to send data on
 * @param target Target chip address.
 * @param alen   Address length (1 for 7-bit, 2 for 10-bit)
 * @param data   Pointer to the data to be sent.
 * @param length Length of the data.
 * @param phase  which phase of a combined operation.
 *
 * @return 0 on Success, -1 on Error
 */
int octeon_i2c_bus_write(int bus, int target, int alen, const uint8_t *data,
			 int length, int phase)
{
	int i, result;
	uint8_t stat;

restart:
	debug("%s(%d, 0x%x, %d, 0x%p, %d, %d)\n", __func__, bus, target, alen,
	      data, length, phase);
	if (phase == 0) {
		result = octeon_i2c_start(bus);
		if (result) {
			debug("%s: Error sending start on bus %d, target 0x%x\n",
			      __func__, bus, target);
			return result;
		}

		/* Send the address which can be 7 or 10 bits */
		switch (alen) {
		case 2:
			octeon_i2c_write_data(bus, (target >> 7) << 1);
			octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB);

			result = octeon_i2c_wait(bus);
			if (result)
				return result;

		case 1:
			octeon_i2c_write_data(bus, (target & 0x7f) << 1);
			octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB);

			result = octeon_i2c_wait(bus);
			if (result) {
				debug("%s: Timed out writing target 0x%x, bus %d\n",
				      __func__, target, bus);
				return result;
			}
			break;
		default:
			break;
		}
	}
	/* Send the data */
	for (i = 0; i < length; i++) {
		/* Check the status */
		stat = octeon_i2c_read_status(bus);
		if (phase == 0 && octeon_i2c_lost_arb(stat)) {
			debug("%s: Lost arbitration on bus %d, restarting...\n",
			      __func__, bus);
			goto restart;
		}

		if ((stat != STAT_TXADDR_ACK) && (stat != STAT_TXDATA_ACK)) {
			debug("%s: bad status before write (0x%x) when writing to target chip address 0x%x\n",
			      __func__, stat, target);
			return -1;
		}

		/* Send data byte */
		octeon_i2c_write_data(bus, data[i]);

		/* enable the bus */
		octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB);

		/* Wait for operation to finish */
		result = octeon_i2c_wait(bus);
		if (result) {
			debug("%s: Operation timed out on bus %d\n",
			      __func__, bus);
			return result;
		}
	}
	return 0;
}

/**
 * Receive data from the bus.
 *
 * @param bus	 The bus to read data from
 * @param target Target address.
 * @param alen   Address length (1 for 7-bit, 2 for 10-bit)
 * @param data   Pointer to the location to store the data.
 * @param length Length of the data.
 * @param phase  which phase of a combined operation.
 *
 * The address is sent over the bus, then the data is read.
 *
 * Returns number of bytes read or -1 on error
 */
static int octeon_i2c_bus_read(int bus, int target, int alen,
			       uint8_t *data, int length, int phase)
{
	int i, result;
	uint8_t stat;

	if (length < 1) {
		printf("%s: Length too short\n", __func__);
		return -1;
	}

restart:
	debug("%s(%d, 0x%x, %d, 0x%p, %d, %d)\n",
	      __func__, bus, target, alen, data, length, phase);
	result = octeon_i2c_start(bus);
	if (result) {
		debug("%s: Error sending start on bus %d, target 0x%x\n",
		      __func__, bus, target);
		return result;
	}

	/* Send the address which can be 7 or 10 bits */
	switch (alen) {
	case 2:
		octeon_i2c_write_data(bus, ((target >> 7) << 1) | 1);
		octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB);

		result = octeon_i2c_wait(bus);
		if (result)
			return result;
	case 1:
		octeon_i2c_write_data(bus, ((target & 0x7f) << 1) | 1);
		octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB);

		result = octeon_i2c_wait(bus);
		if (result) {
			debug("%s: Timed out writing target 0x%x, bus %d\n",
			      __func__, target, bus);
			return result;
		}
		break;
	default:
		printf("%s: Invalid address length %d\n", __func__, alen);
		return -1;
	}

	for (i = 0; i < length; i++) {
		stat = octeon_i2c_read_status(bus);
		debug("  status %d: 0x%x\n", i, stat);
		if (phase == 0 && octeon_i2c_lost_arb(stat)) {
			debug("%s: Lost arbitration on bus %d, restarting...\n",
			      __func__, bus);
			goto restart;
		}

		if ((stat != STAT_RXDATA_ACK) &&
		    (stat != STAT_RXADDR_ACK)) {
			debug("%s: bad status before read (0x%x), address 0x%x\n",
			      __func__, stat, target);
			return -1;
		}

		if (i + 1 < length)
			octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB | TWSI_CTL_AAK);
		else
			octeon_i2c_write_ctl(bus, TWSI_CTL_ENAB);

		result = octeon_i2c_wait(bus);
		if (result) {
			debug("%s: Operation timed out on bus %d\n",
			      __func__, bus);
			return -1;
		}

		data[i] = octeon_i2c_read_data(bus);
		debug("   data: 0x%x\n", data[i]);
	}
	return i;
}

/**
 * Reads bytes from eeprom and copies to DRAM.
 * Only supports address size of 2 (16 bit internal address.)
 *
 * @param bus    I2C bus
 * @param chip   chip address
 * @param addr   internal address
 * @param alen   address length
 * @param buffer memory buffer pointer
 * @param len    number of bytes to read
 *
 * @return 0 on Success
 *         1 on Failure
 */
int octeon_i2c_read(unsigned int bus, uint8_t chip, uint addr,
			   int alen, uint8_t *buffer, int len)
{
	uint8_t addr_buf[2];
	int result = -1;

	debug("%s: Reading device: %#04x address %#04x, alen=%d, len=%d\n",
	      __func__, chip, addr, alen, len);
	debug("  bus: %d\n", bus);

	if (alen > 2  || (len && !buffer))
		return 1;

	switch (alen) {
	case 2:
		addr_buf[0] = (addr >> 8) & 0xff;
		addr_buf[1] = addr & 0xff;
		break;
	case 1:
		addr_buf[0] = addr & 0xff;
		break;
	case 0:
		break;
	default:
		printf("%s: Invalid address length %d\n", __func__, alen);
		goto out;
	}

	result = octeon_i2c_bus_write(bus, chip, 1, addr_buf, alen, 0);
	if (result < 0) {
		debug("%s: Error sending address: 0x%x, addr len: %d\n",
		      __func__, addr, alen);
		goto out;
	}

	result = octeon_i2c_bus_read(bus, chip, 1, buffer, len, 1);
	if (result < 0) {
		printf("%s: Error reading bytes\n", __func__);
		goto out;
	}

	result = octeon_i2c_bus_stop(bus);
	if (result) {
		debug("%s: Error sending stop to bus %d\n", __func__, bus);
	}

out:
	debug("%s: result: %d\n", __func__, result);
	return (result < 0) ? 1 : 0;
}

/**
 * Reads bytes from memory and copies to eeprom.
 * Only supports address size of 2 (16 bit internal address.)
 *
 * We can only write two bytes at a time to the eeprom, so in some cases
 * we need to to a read/modify/write.
 *
 * Note: can't do single byte write to last address in EEPROM
 *
 * @param adap   i2c adapter
 * @param chip   chip address
 * @param addr   internal address
 * @param alen   address length, must be 0, 1, or 2
 * @param buffer memory buffer pointer
 * @param len    number of bytes to write
 *
 * @return 0 on Success
 *         1 on Failure
 */
int octeon_i2c_write(unsigned int bus, uint8_t chip, uint addr,
			    int alen, uint8_t *buffer, int len)
{
	uint8_t addr_buf[2];
	int result;

	debug("%s: Writing device: %#04x address %#04x data %#04x,"
	      " alen=%d, len=%d\n", __func__, chip, addr, *buffer, alen, len);
	debug("  bus: %d\n", bus);
	if (alen > 2 || (len && !buffer))
		return 1;

	switch (alen) {
	case 2:
		addr_buf[0] = (addr >> 8) & 0xff;
		addr_buf[1] = addr & 0xff;
		break;
	case 1:
		addr_buf[0] = addr & 0xff;
		break;
	case 0:
		break;
	default:
		printf("%s: Invalid address length %d\n", __func__, alen);
		return 1;
	}
	result = octeon_i2c_bus_write(bus, chip, 1, addr_buf, alen, 0);
	if (result < 0) {
		debug("%s: Error sending address\n", __func__);
		return 1;
	}
	result = octeon_i2c_bus_write(bus, chip, 1, buffer, len, 1);
	if (result < 0)
		return 1;

	result = octeon_i2c_bus_stop(bus);
	debug("%s: result: %d\n", __func__, result);
	return (result < 0) ? 1 : 0;

}
