/*
 * OCTEON SPI driver
 *
 * Copyright (C) 2013-2014 Cavium, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <spi-config.h>
#include <tinylibc.h>
#include <cvmx.h>
#include <cvmx-mpi-defs.h>
#include <spi.h>

#define OCTEON_SPI_MAX_BYTES		9
#define OCTEON_SPI_MAX_CLOCK_HZ_7XXX	(200000000UL)

static struct octeon_spi spi_info[OCTEON_MAX_SPI_DEVICES];

/**
 * Initializes any SPI data structures before use.
 */
void spi_init(void)
{
	memset(spi_info, 0, sizeof(spi_info));
}

/**
 * Waits until the SPI bus is ready
 *
 * @param[in]	ospi	Pointer to SPI slave data structure
 */
static void octeon_spi_wait_ready(const struct octeon_spi *ospi)
{
	union cvmx_mpi_sts mpi_sts;

	do {
		mpi_sts.u64 = cvmx_read_csr_node(ospi->node, CVMX_MPI_STS);
	} while (mpi_sts.s.busy != 0);
}

/**
 * Sets up the mpi_cfg CSR
 *
 * @param[in]	ospi	Pointer to SPI slave data structure
 * @param	cs	Chip select to enable or -1 to not enable any.
 */
static u64 octeon_spi_set_mpicfg(const struct octeon_spi *ospi, int cs)
{
	union cvmx_mpi_cfg mpi_cfg;
	int cpha, cpol;
	cpha = !!(ospi->mode & SPI_CPHA);
	cpol = !!(ospi->mode & SPI_CPOL);

	mpi_cfg.u64 = 0;
	mpi_cfg.s.clkdiv = ospi->clkdiv;
	mpi_cfg.s.cshi = !!(ospi->mode & SPI_CS_HIGH);
	mpi_cfg.s.lsbfirst = !!(ospi->mode & SPI_LSB_FIRST);
	mpi_cfg.s.wireor = !!(ospi->mode & SPI_3WIRE);
	mpi_cfg.s.idlelo = cpha != cpol;
	mpi_cfg.s.cslate = cpha;
	mpi_cfg.s.enable = 1;

	switch (cs) {
	case 0:
		mpi_cfg.s.csena0 = 1;
		break;
	case 1:
		mpi_cfg.s.csena1 = 1;
		break;
	case 2:
		mpi_cfg.s.csena2 = 1;
		break;
	case 3:
		mpi_cfg.s.csena3 = 1;
		break;
	default:
		break;
	}

	return mpi_cfg.u64;
}

/**
 * Set up a SPI slave device
 *
 * @param node		Node number for 78XX
 * @param cs		Chip select number (0-3)
 * @param max_hz	Maximum speed for device
 * @param mode		Device mode flags
 *
 * @return		Pointer to slave data structure or NULL if failure
 */
struct octeon_spi *spi_setup_slave(u8 node, u8 cs, u32 max_hz, u8 mode)
{
	struct octeon_spi *ospi = NULL;
	int i;

	for (i = 0; i < OCTEON_MAX_SPI_DEVICES; i++)
		 if (!spi_info[i].setup) {
			 ospi = &spi_info[i];
			 break;
		 }
	if (!ospi) {
		puts("Could not find free SPI slot\n");
		return NULL;
	}

	max_hz = min(max_hz, OCTEON_SPI_MAX_CLOCK_HZ_7XXX);
	ospi->clkdiv =
		octeon_clock_get_rate_node(node, CVMX_CLOCK_SCLK) / (2 * max_hz);
	ospi->cs = cs;
	ospi->max_speed_hz = max_hz;
	ospi->mode = mode;
	ospi->bits_per_word = SPI_DEFAULT_WORDLEN;
	ospi->setup = 1;
	ospi->node = node;

	cvmx_write_csr_node(ospi->node, CVMX_MPI_CFG,
			    octeon_spi_set_mpicfg(ospi, -1));

	return ospi;
}

/**
 * Free up a slave device
 *
 * @param[in,out]	ospi	Pointer to slave data structure to free
 */
void spi_free_slave(struct octeon_spi *ospi)
{
	cvmx_write_csr_node(ospi->node, CVMX_MPI_CFG, 0);
	ospi->setup = 0;
	ospi->claimed = 0;
}

/**
 * Claim the slave device
 *
 * @param[in,out]	ospi	Pointer to slave to claim
 *
 * @return	0 for success
 */
int spi_claim_bus(struct octeon_spi *ospi)
{
	union cvmx_mpi_cfg mpi_cfg;
	mpi_cfg.u64 = cvmx_read_csr_node(ospi->node, CVMX_MPI_CFG);
	mpi_cfg.s.tritx = 0;
	mpi_cfg.s.enable = 1;
	cvmx_write_csr_node(ospi->node, CVMX_MPI_CFG, mpi_cfg.u64);
	ospi->claimed = 1;
	return 0;
}

/**
 * Releases the slave device
 *
 * @param[in,out]	ospi	Pointer to slave to release
 */
void spi_release_bus(struct octeon_spi *ospi)
{
	union cvmx_mpi_cfg mpi_cfg;
	mpi_cfg.u64 = cvmx_read_csr_node(ospi->node, CVMX_MPI_CFG);
	mpi_cfg.s.tritx = !!(ospi->mode & SPI_3WIRE);
	cvmx_write_csr_node(ospi->node, CVMX_MPI_CFG, mpi_cfg.u64);
	ospi->claimed = 0;
}

/**
 * Perform a data transfer operation to a SPI slave device
 *
 * @param[in]	ospi	Slave device to transfer data with
 * @param	bitlen	Number of bits to transfer, must be a multiple of 8.
 * @param[in]	dout	Pointer to buffer to send (can be NULL)
 * @param[out]	din	Pointer to buffer for received data (can be NULL)
 * @param	flags	SPI transfer flags
 *
 * @return	0 for success
 */
int spi_xfer(struct octeon_spi *ospi, u32 bitlen, const void *dout, void *din,
	     u32 flags)
{
	union cvmx_mpi_tx	mpi_tx;
	union cvmx_mpi_cfg	mpi_cfg;
	const u8		*tx_data = dout;
	u8			*rx_data = din;
	u32			len = bitlen / 8;
	u32			i;

	if (flags & SPI_XFER_BEGIN) {
		/* do nothing */
	}

	mpi_cfg.u64 = octeon_spi_set_mpicfg(ospi, ospi->cs);
	if (mpi_cfg.u64 != cvmx_read_csr_node(ospi->node, CVMX_MPI_CFG))
		cvmx_write_csr_node(ospi->node, CVMX_MPI_CFG, mpi_cfg.u64);

	while (len > OCTEON_SPI_MAX_BYTES) {
		if (tx_data) {
			for (i = 0; i < OCTEON_SPI_MAX_BYTES; i++)
				cvmx_write_csr_node(ospi->node,
						    CVMX_MPI_DATX(i),
						    *(tx_data++));
		}
		mpi_tx.u64 = 0;
		mpi_tx.s.csid = ospi->cs;
		mpi_tx.s.leavecs = 1;
		mpi_tx.s.txnum = tx_data ? OCTEON_SPI_MAX_BYTES : 0;
		mpi_tx.s.totnum = OCTEON_SPI_MAX_BYTES;
		cvmx_write_csr_node(ospi->node, CVMX_MPI_TX, mpi_tx.u64);

		octeon_spi_wait_ready(ospi);

		if (rx_data)
			for (i = 0; i < OCTEON_SPI_MAX_BYTES; i++)
				*(rx_data++) = cvmx_read_csr_node(ospi->node,
								  CVMX_MPI_DATX(i));
		len -= OCTEON_SPI_MAX_BYTES;
	}

	if (tx_data)
		for (i = 0; i < len; i++)
			cvmx_write_csr_node(ospi->node,
					    CVMX_MPI_DATX(i),
					    *(tx_data++));
	mpi_tx.u64 = 0;
	mpi_tx.s.csid = ospi->cs;
	mpi_tx.s.leavecs = !(flags & SPI_XFER_END);
	mpi_tx.s.txnum = tx_data ? len : 0;
	mpi_tx.s.totnum = len;
	cvmx_write_csr_node(ospi->node, CVMX_MPI_TX, mpi_tx.u64);

	octeon_spi_wait_ready(ospi);

	if (rx_data)
		for (i = 0; i < len; i++)
			*(rx_data++) = cvmx_read_csr_node(ospi->node,
							  CVMX_MPI_DATX(i));

	return 0;
}

#define SPI_READ_ID	0x9f

int spi_read_id(u8 node, u8 cs, u8 *id)
{
	struct octeon_spi *ospi;
	static const u8 cmd = SPI_READ_ID;

	ospi = spi_setup_slave(node, cs, CONFIG_SPI_FREQUENCY, SPI_MODE_0);
	if (!ospi) {
		puts("Error setting up slave\n");
		return -1;
	}
	spi_claim_bus(ospi);

	if (spi_xfer(ospi, 8, &cmd, NULL, SPI_XFER_BEGIN))
		return -1;
	if (spi_xfer(ospi, 8*5, NULL, id, SPI_XFER_END))
		return -1;
	return 0;
}

#ifdef CONFIG_SPI_EEPROM_M95XXX

#define SPI_EEPROM_WREN		0x06
#define SPI_EEPROM_RDSR		0x05
#define SPI_EEPROM_READ		0x03
#define SPI_EEPROM_WRITE	0x02

/**
 * Reads len bytes into the buffer starting at offset addr
 *
 * @param	node	node number to read from
 * @param	cs	chip select to read from
 * @param[in]	addr	array of alen address bytes
 * @param	alen	Size of address in bytes
 * @param[out]	buffer	Buffer to read data into
 * @param	len	number of bytes to read
 *
 * @return	number of bytes read or -1 if error
 */
int spi_read(u8 node, u8 cs, const u8 *addr, int alen, void *buffer, int len)
{
	struct octeon_spi *ospi;
	static const u8 cmd = SPI_EEPROM_READ;

	ospi = spi_setup_slave(node, cs, CONFIG_SPI_FREQUENCY, SPI_MODE_0);
	if (!ospi) {
		puts("Error setting up slave\n");
		return -1;
	}
	spi_claim_bus(ospi);

	if (spi_xfer(ospi, 8, &cmd, NULL, SPI_XFER_BEGIN)) {
		puts("Error with SPI read cmd send\n");
		return -1;
	}

	/* If alen == 3, addr[0] is the block number, we never use it here.
	 * All we need are the lower 16 bits.
	 */
	if (alen == 3)
		 addr++;

	/* address and data */
	if (spi_xfer(ospi, 16, addr, NULL, 0))
		 return -1;
	if (spi_xfer(ospi, 8 * len, NULL, buffer, SPI_XFER_END))
		 return -1;

	spi_release_bus(ospi);
	spi_free_slave(ospi);

	return len;
}
#endif /* CONFIG_SPI_EEPROM_M95XXX */

#ifdef CONFIG_SPI_NOR
#define SPI_NOR_READ_ARRAY_SLOW		0x03
#define SPI_NOR_READ_ARRAY_FAST		0x0b
#define SPI_NOR_READ_ID			0x9f
#define SPI_NOR_READ_STATUS		0x05
#define SPI_NOR_READ_STATUS1		0x35
#define SPI_NOR_READ_CONFIG		0x35
#define SPI_NOR_FLAG_STATUS		0x70
/**
 * Reads len bytes into the buffer starting at offset addr
 *
 * @param	node	node number to read from
 * @param	cs	chip select to read from
 * @param[in]	addr	array of alen address bytes
 * @param	alen	Size of address in bytes
 * @param[out]	buffer	Buffer to read data into
 * @param	len	number of bytes to read
 *
 * @return	number of bytes read or -1 if error
 */
int spi_read(u8 node, u8 cs, const u8 *addr, int alen, void *buffer, int len)
{
	struct octeon_spi *ospi;
	u8 cmd[6];

	if (addr > 2) {
		cmd[0] = SPI_NOR_READ_ARRAY_FAST;
		cmd[1] = addr[0];
		cmd[2] = addr[1];
		cmd[3] = addr[2];
		if (alen > 3) {
			cmd[4] = addr[3];
			cmd[5] = 0x00;
		} else {
			cmd[4] = 0x00;
		}
	}

	ospi = spi_setup_slave(node, cs, CONFIG_SPI_FREQUENCY, SPI_MODE_0);
	if (!ospi) {
		puts("Error setting up slave\n");
		return -1;
	}
	spi_claim_bus(ospi);

	debug("%s(%d, %d, %p, %d, %p, 0x%x): cmd: %02x:%02x:%02x:%02x:%02x\n",
	      __func__, node, cs, addr, alen, buffer, len, cmd[0], cmd[1],
	      cmd[2], cmd[3], cmd[4]);

	if (alen == 2) {
		if (spi_xfer(ospi, 16, addr, NULL, 0))
			return -1;
	} else {
		if (spi_xfer(ospi, 8 * (alen + 2), cmd, NULL, SPI_XFER_BEGIN))
			return -1;
	}

	if (spi_xfer(ospi, 8 * len, NULL, buffer, SPI_XFER_END))
		return -1;

	spi_release_bus(ospi);
	spi_free_slave(ospi);

	return len;
}
#endif /* CONFIG_SPI_NOR */
