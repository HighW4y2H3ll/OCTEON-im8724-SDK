/*
 * Common SPI Interface: Controller-specific definitions
 *
 * (C) Copyright 2001
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SPI_H_
#define _SPI_H_

/* SPI mode flags */
#define	SPI_CPHA	0x01			/* clock phase */
#define	SPI_CPOL	0x02			/* clock polarity */
#define	SPI_MODE_0	(0|0)			/* (original MicroWire) */
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			/* CS active high */
#define	SPI_LSB_FIRST	0x08			/* per-word bits-on-wire */
#define	SPI_3WIRE	0x10			/* SI/SO signals shared */
#define	SPI_LOOP	0x20			/* loopback mode */
#define	SPI_SLAVE	0x40			/* slave mode */
#define	SPI_PREAMBLE	0x80			/* Skip preamble bytes */

/* SPI transfer flags */
#define SPI_XFER_BEGIN		0x01	/* Assert CS before transfer */
#define SPI_XFER_END		0x02	/* Deassert CS after transfer */
#define SPI_XFER_MMAP		0x08	/* Memory Mapped start */
#define SPI_XFER_MMAP_END	0x10	/* Memory Mapped End */
#define SPI_XFER_ONCE		(SPI_XFER_BEGIN | SPI_XFER_END)

/* Header byte that marks the start of the message */
#define SPI_PREAMBLE_END_BYTE	0xec

#define SPI_DEFAULT_WORDLEN 8

/** Maximum number of SPI devices that can be setup */
#define OCTEON_MAX_SPI_DEVICES		4

/** SPI device data structure */
struct octeon_spi {
	u32 max_speed_hz;	/** Maximum device speed in hz */
	u16 clkdiv;		/** Clock divisor for device */
	u8 mode;		/** Device mode flags */
	u8 bits_per_word;	/** Usually 8 */
	u8 cs:4;		/** Chip select */
	u8 claimed:1;		/** Whether or not device is claimed */
	u8 setup:1;		/** Whether or not device is setup (allocated) */
	u8 node:2;		/** Node number for device */
};

/**
 * Initializes any SPI data structures before use.
 */
void spi_init(void);

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
struct octeon_spi *spi_setup_slave(u8 node, u8 cs, u32 max_hz, u8 mode);

/**
 * Free up a slave device
 *
 * @param[in,out]	ospi	Pointer to slave data structure to free
 */
void spi_free_slave(struct octeon_spi *ospi);

/**
 * Claim the slave device
 *
 * @param[in,out]	ospi	Pointer to slave to claim
 *
 * @return	0 for success
 */
int spi_claim_bus(struct octeon_spi *ospi);

/**
 * Releases the slave device
 *
 * @param[in,out]	ospi	Pointer to slave to release
 */
void spi_release_bus(struct octeon_spi *ospi);

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
	     u32 flags);

/**
 * Reads the ID of the SPI device
 *
 * @param	node	node number to read from
 * @param	cs	chip select to read from
 * @param[out]	id	ID read
 *
 * @return	-1 on error, 0 for success
 */
int spi_read_id(u8 node, u8 cs, u8 *id);

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
int spi_read(u8 node, u8 cs, const u8 *addr, int alen, void *buffer, int len);

#endif /* _SPI_H_ */
