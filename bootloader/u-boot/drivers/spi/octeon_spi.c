/*
 * OCTEON SPI driver
 *
 * Copyright (C) 2013 Cavium, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-mpi-defs.h>
#include <asm/arch/lib_octeon.h>

DECLARE_GLOBAL_DATA_PTR;

#define OCTEON_SPI_MAX_BYTES		9
#define OCTEON_SPI_MAX_CLOCK_HZ		16000000	/** Max clock speed */
#define OCTEON_SPI_MAX_CLOCK_HZ_7XXX	200000000	/** Max clock for O3 */
#define OCTEON_SPI_MAX_CHIP_SEL		1

/** Local driver datastructure */
struct octeon_spi {
	struct spi_slave slave;	/** Parent slave data structure */
	u32 max_speed_hz;	/** Maximum device speed in hz */
	u32 mode;		/** Device mode */
	u32 clkdiv;		/** Clock divisor for device speed */
	u8 bits_per_word;	/** Bits per word, usually 8 */
};

/**
 * Wait until the SPI bus is ready
 *
 * @param[in] slave	Slave data structure
 */
static void octeon_spi_wait_ready(const struct spi_slave *slave)
{
	union cvmx_mpi_sts mpi_sts;

	while (1) {
		mpi_sts.u64 = cvmx_read_csr_node(slave->bus, CVMX_MPI_STS);
		if (mpi_sts.s.busy == 0)
			return;
		udelay(1);
	}
}

/**
 * Initializes SPI interface
 */
void spi_init(void)
{

}

/**
 * Initializes SPI interface before relocation
 * NOTE: We don't work before relocation.
 */
void spi_init_f(void)
{

}

/**
 * Configure the mpi_cfg CSR according to the device configuration.
 *
 * @param[in]	ospi	Octeon SPI data structure
 * @param	cs	Chip select to enable, -1 to not enable any
 *
 * @return	64-bit value for mpi_cfg CSR.
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
 * @param bus		Bus number.  For 78XX this is the node number.
 * @param cs		Chip select number (0-3)
 * @param max_hz	Maximum speed for device
 * @param mode		Device mode flags
 *
 * @return		Pointer to slave data structure or NULL if failure
 */
struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct octeon_spi	*ospi;

	debug("%s(%u, %u, %u, %u)\n", __func__, bus, cs, max_hz, mode);

	if (!gd->flags & GD_FLG_RELOC) {
		debug("%s: relocation required\n", __func__);
		return NULL;
	}
	if (!spi_cs_is_valid(bus, cs)) {
		printf("Invalid SPI chip select %d\n", cs);
		return NULL;
	}

	ospi = calloc(sizeof(*ospi), 1);
	if (!ospi) {
		printf("%s: Cannot allocate memory\n", __func__);
		return NULL;
	}

	if (OCTEON_IS_OCTEON3())
		max_hz = min(max_hz, OCTEON_SPI_MAX_CLOCK_HZ_7XXX);
	else
		max_hz = min(max_hz, OCTEON_SPI_MAX_CLOCK_HZ);

	ospi->clkdiv = gd->bus_clk / (2 * max_hz);
	debug("%s: clockdiv: %d\n", __func__, ospi->clkdiv);
	ospi->slave.bus = bus;
	ospi->slave.cs = cs;
	ospi->max_speed_hz = max_hz;
	ospi->mode = mode;
	ospi->bits_per_word = SPI_DEFAULT_WORDLEN;

	cvmx_write_csr_node(bus, CVMX_MPI_CFG, octeon_spi_set_mpicfg(ospi, -1));

	return &ospi->slave;
}

/**
 * Free up a slave device
 *
 * @param[in,out]	slave	Pointer to slave data structure to free
 */
void spi_free_slave(struct spi_slave *slave)
{
	struct octeon_spi *ospi = container_of(slave, struct octeon_spi, slave);
	debug("%s(0x%p)\n", __func__, slave);
	cvmx_write_csr_node(slave->bus, CVMX_MPI_CFG, 0);
	free(ospi);
}

/**
 * Claim the slave device
 *
 * @param[in]	slave	Pointer to slave to claim
 *
 * @return	0 for success
 */
int spi_claim_bus(struct spi_slave *slave)
{
	union cvmx_mpi_cfg	mpi_cfg;

	debug("%s(0x%p)\n", __func__, slave);
	mpi_cfg.u64 = cvmx_read_csr_node(slave->bus, CVMX_MPI_CFG);
	mpi_cfg.s.tritx = 0;
	mpi_cfg.s.enable = 1;
	cvmx_write_csr_node(slave->bus, CVMX_MPI_CFG, mpi_cfg.u64);
	return 0;
}

/**
 * Releases the slave device
 *
 * @param[in]	slave	Pointer to slave to release
 */
void spi_release_bus(struct spi_slave *slave)
{
	struct octeon_spi *ospi = container_of(slave, struct octeon_spi, slave);
	union cvmx_mpi_cfg mpi_cfg;

	debug("%s(0x%p)\n", __func__, slave);
	mpi_cfg.u64 = cvmx_read_csr_node(slave->bus, CVMX_MPI_CFG);
	mpi_cfg.s.tritx = (ospi->mode & SPI_3WIRE) ? 1 : 0;
	cvmx_write_csr_node(slave->bus, CVMX_MPI_CFG, mpi_cfg.u64);
}

/**
 * Perform a data transfer operation to a SPI slave device
 *
 * @param[in]	slave	Slave device to transfer data with
 * @param	bitlen	Number of bits to transfer, must be a multiple of 8.
 * @param[in]	dout	Pointer to buffer to send (can be NULL)
 * @param[out]	din	Pointer to buffer for received data (can be NULL)
 * @param	flags	SPI transfer flags
 *
 * @return	0 for success
 */
int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	union cvmx_mpi_tx	mpi_tx;
	union cvmx_mpi_cfg	mpi_cfg;
	struct octeon_spi *ospi = container_of(slave, struct octeon_spi, slave);
	const unsigned char	*tx_data = dout;
	unsigned char		*rx_data = din;
	unsigned int		len = bitlen / 8;
	int			i;

	debug("%s(0x%p, %u, 0x%p, 0x%p, %lu)\n", __func__, slave, bitlen,
	      dout, din, flags);

	if (flags & SPI_XFER_BEGIN) {
		debug("%s: SPI transfer begin\n", __func__);
		/* Do nothing */
	}

	debug("%s: mode: 0x%x, clkdiv: %u\n", __func__,
	      ospi->mode, ospi->clkdiv);
	mpi_cfg.u64 = octeon_spi_set_mpicfg(ospi, slave->cs);

	if (mpi_cfg.u64 != cvmx_read_csr_node(slave->bus, CVMX_MPI_CFG)) {
		debug("%s: MPI_CFG: 0x%llx\n", __func__, mpi_cfg.u64);
		cvmx_write_csr_node(slave->bus, CVMX_MPI_CFG, mpi_cfg.u64);
	}

	while (len > OCTEON_SPI_MAX_BYTES) {
		if (tx_data)
			for (i = 0; i < OCTEON_SPI_MAX_BYTES; i++) {
				u8 d = *tx_data++;
				debug("%s: (tx) MPI_DATX(%d): 0x%x\n",
				      __func__, i, d);
				cvmx_write_csr_node(slave->bus,
						    CVMX_MPI_DATX(i), d);
			}

		mpi_tx.u64 = 0;
		mpi_tx.s.csid = slave->cs;
		mpi_tx.s.leavecs = 1;
		mpi_tx.s.txnum = tx_data ? OCTEON_SPI_MAX_BYTES : 0;
		mpi_tx.s.totnum = OCTEON_SPI_MAX_BYTES;
		debug("%s: MPI_TX: 0x%llx\n", __func__, mpi_tx.u64);
		cvmx_write_csr_node(slave->bus, CVMX_MPI_TX, mpi_tx.u64);

		octeon_spi_wait_ready(slave);

		if (rx_data)
			for (i = 0; i < OCTEON_SPI_MAX_BYTES; i++) {
				u64 v = cvmx_read_csr_node(slave->bus,
							   CVMX_MPI_DATX(i));
				debug("%s: (rx) MPI_DATX(%d): 0x%llx\n",
				      __func__, i, v);
				*rx_data++ = (u8)v;
			}

		len -= OCTEON_SPI_MAX_BYTES;
	}

	if (tx_data)
		for (i = 0; i < len; i++) {
			u8 d = *tx_data++;
			debug("%s: (tx) MPI_DATX(%d): 0x%x\n",
			      __func__, i, d);
			cvmx_write_csr_node(slave->bus, CVMX_MPI_DATX(i), d);
		}

	mpi_tx.u64 = 0;
	mpi_tx.s.csid = slave->cs;
	mpi_tx.s.leavecs = !(flags & SPI_XFER_END);
	mpi_tx.s.txnum = tx_data ? len : 0;
	mpi_tx.s.totnum = len;
	debug("%s: MPI_TX: 0x%llx\n", __func__, mpi_tx.u64);
	cvmx_write_csr_node(slave->bus, CVMX_MPI_TX, mpi_tx.u64);

	octeon_spi_wait_ready(slave);

	if (rx_data)
		for (i = 0; i < len; i++) {
			u64 v = cvmx_read_csr_node(slave->bus, CVMX_MPI_DATX(i));
			debug("%s: (rx) MPI_DATX(%d): 0x%llx\n",
			      __func__, i, v);
			*rx_data++ = (u8)v;
		}
	return 0;
}

/**
 * Returns if the SPI bus and chip select are valid
 *
 * @param	bus	bus to check
 * @param	cs	chip select to check
 *
 * @return	1 if valid, 0 if invalid
 */
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	debug("%s(%u, %u)\n", __func__, bus, cs);
	return (!bus && (cs >= 0 && cs <= 3));
}

/**
 * Activates the SPI device
 *
 * @param[in]	slave	Pointer to slave data structure
 */
void spi_cs_activate(struct spi_slave *slave)
{

}

/**
 * Deactivates the SPI device
 *
 * @param[in]	slave	Pointer to slave data structure
 */
void spi_cs_deactivate(struct spi_slave *slave)
{

}
