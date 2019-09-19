/*
 * Copyright 2013 Cavium, Inc. <support@cavium.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include "config.h"
#include <stdint.h>
#include "cvmx.h"
#include "cvmx-mpi-defs.h"
#include "cvmx-l2c-defs.h"
#include "cvmx-ipd-defs.h"
#include "cvmx-clock.h"
#include "cvmx-rst-defs.h"

#define CONFIG_SYS_HZ		1000
#define CYCLES_PER_JIFFY	(get_cpu_clk() / CONFIG_SYS_HZ)

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
#define SPI_XFER_BEGIN	0x01			/* Assert CS before transfer */
#define SPI_XFER_END	0x02			/* Deassert CS after transfer */

/* Header byte that marks the start of the message */
#define SPI_PREAMBLE_END_BYTE	0xec

#define OCTEON_SPI_MAX_BYTES	9
#define OCTEON_SPI_MAX_CLOCK_HZ	16000000
#define OCTEON_SPI_MAX_CHIP_SEL	1

#ifndef CONFIG_SF_DEFAULT_SPEED
# define CONFIG_SF_DEFAULT_SPEED	1000000
#endif
#ifndef CONFIG_SF_DEFAULT_MODE
# define CONFIG_SF_DEFAULT_MODE		SPI_MODE_3
#endif
#ifndef CONFIG_SF_DEFAULT_CS
# define CONFIG_SF_DEFAULT_CS		0
#endif
#ifndef CONFIG_SF_DEFAULT_BUS
# define CONFIG_SF_DEFAULT_BUS		0
#endif

/* Common parameters -- kind of high, but they should only occur when there
 * is a problem (and well your system already is broken), so err on the side
 * of caution in case we're dealing with slower SPI buses and/or processors.
 */
#define SPI_FLASH_PROG_TIMEOUT		(2 * CONFIG_SYS_HZ)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT	(5 * CONFIG_SYS_HZ)
#define SPI_FLASH_SECTOR_ERASE_TIMEOUT	(10 * CONFIG_SYS_HZ)

/* Common commands */
#define CMD_READ_ID			0x9f

#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b

#define CMD_WRITE_STATUS		0x01
#define CMD_PAGE_PROGRAM		0x02
#define CMD_WRITE_DISABLE		0x04
#define CMD_READ_STATUS			0x05
#define CMD_FLAG_STATUS			0x70
#define CMD_WRITE_ENABLE		0x06
#define CMD_ERASE_4K			0x20
#define CMD_ERASE_32K			0x52
#define CMD_ERASE_64K			0xd8
#define CMD_ERASE_CHIP			0xc7

#define SPI_FLASH_16MB_BOUN		0x1000000

/* Manufacture ID's */
#define SPI_FLASH_SPANSION_IDCODE0	0x01
#define SPI_FLASH_STMICRO_IDCODE0	0x20
#define SPI_FLASH_WINBOND_IDCODE0	0xef

#ifdef CONFIG_SPI_FLASH_BAR
/* Bank addr access commands */
# define CMD_BANKADDR_BRWR		0x17
# define CMD_BANKADDR_BRRD		0x16
# define CMD_EXTNADDR_WREAR		0xC5
# define CMD_EXTNADDR_RDEAR		0xC8
#endif

/* Common status */
#define STATUS_WIP			0x01
#define STATUS_PEC			0x80

struct octeon_spi {
	unsigned int	max_speed_hz;	/** Max speed in HZ */
	unsigned int	mode;		/** SPI mode */
	unsigned int	cs;		/** Chip select */
	unsigned int	max_write_size;	/** Maximum write size */
	unsigned char	bits_per_word;	/** Bits per word */
};

struct spi_flash {
	struct octeon_spi *spi;

	/** Total flash size */
	u32		size;
	/** Write (page) size */
	u32		page_size;
	/** Erase (sector) size */
	u32		sector_size;
#ifdef CONFIG_SPI_FLASH_BAR
	/** Bank read cmd */
	u8		bank_read_cmd;
	/** Bank write cmd */
	u8		bank_write_cmd;
	/** Current flash bank */
	u8		bank_curr;
#endif
	/** Poll cmd - for flash erase/program */
	u8		poll_cmd;

	void		*memory_map;	/** Address of read-only SPI flash access */
	int		(*read)(struct spi_flash *flash, u32 offset,
				size_t len, void *buf);
	int		(*write)(struct spi_flash *flash, u32 offset,
				size_t len, const void *buf);
	int		(*erase)(struct spi_flash *flash, u32 offset,
				size_t len);
};

/************ Utility Functions *******************/

/**
 * Returns the CPU clock in hz
 *
 * @return CPU clock speed in hz
 */
static inline uint64_t get_cpu_clk(void)
{
	union cvmx_rst_boot rst_boot;

	rst_boot.u64 = cvmx_read_csr_node(0, CVMX_RST_BOOT);

	return rst_boot.s.c_mul * 50000000;
}

/**
 * Returns the I/O clock rate in HZ
 *
 * @return I/O clock rate in HZ
 */
static inline uint64_t octeon_get_ioclk_hz(void)
{
	cvmx_rst_boot_t rst_boot;

	rst_boot.u64 = cvmx_read_csr(CVMX_RST_BOOT);
	return rst_boot.s.pnr_mul * 50000000;
}

/**************** SPI driver functions ****************/

/**
 * Wait until the current SPI operation is complete
 */
static inline void octeon_spi_wait_ready(void)
{
	union cvmx_mpi_sts mpi_sts;

	do {
		mpi_sts.u64 = cvmx_read_csr(CVMX_MIP_STS);
	} while (mpi_sts.s.busy != 0);
}

static int timer_init(void)
{
	write_c0_compare(read_c0_count() + CYCLES_PER_JIFFY);
}

static ulong get_timer(ulong base)
{
	unsigned int count;
	unsigned int expirelo = read_c0_compare();

	/* Check to see if we have missed any timestamps. */
	count = read_c0_count();
	while ((count - expirelo) < 0x7fffffff) {
		expirelo += CYCLES_PER_JIFFY;
		timestamp++;
	}
	write_c0_compare(expirelo);
	return (timestamp - base);
}

/**
 * Claim the SPI bus
 *
 * @param spi	Pointer to SPI data structure (unused)
 *
 * @return 0 for success
 */
static int spi_claim_bus(struct octeon_spi *spi)
{
	union cvmx_mpi_cfg mpi_cfg;

	mpi_cfg.u64 = cvmx_read_csr(CVMX_MPI_CFG);
	mpi_cfg.s.tritx = 0;
	mpi_cfg.s.enable = 1;
	cvmx_write_csr(CVMX_MPI_CFG, mpi_cfg.u64);
	return 0;
}

/**
 * Releases the SPI bus
 *
 * @param spi	Pointer to SPI data structure
 */
static void spi_release_bus(struct octeon_spi *spi)
{
	union cvmx_mpi_cfg mpi_cfg;

	mpi_cfg.u64 = cvmx_read_csr(CVMX_MPI_CFG);
	mpi_cfg.s.tritx = (spi->mode & SPI_3WIRE) ? 1 : 0;
	cvmx_write_csr(CVMX_MPI_CFG, mpi_cfg.u64);
}

/**
 * Performs a data transfer operation
 *
 * @param spi		Pointer to SPI data structure
 * @param bitlen
 * @param dout		Pointer to buffer to send
 * @param din		Pointer to buffer to receive
 * @param flags		Flags for operation
 *
 * @return 0 for success, failure otherwise
 */
static int spi_xfer(struct octeon_spi *spi, unsigned int bitlen,
		    const void *dout, void *din, unsigned long flags)
{
	union cvmx_mpi_tx	mpi_tx;
	union cvmx_mpi_cfg	mpi_cfg;
	const unsigned char	*tx_data = dout;
	unsigned char		*rx_data = din;
	unsigned int		len = bitlen / 8;
	int			i;
	unsigned int		clkdiv;
	unsigned int		speed_hz;
	unsigned int		mode;
	int			cpha, cpol;

	if (flags & SPI_XFER_BEGIN) {
		/* Do nothing */
	}

	speed_hz = spi->max_speed_hz;
	mode = spi->mode;
	cpha = !!(mode & SPI_CPHA);
	cpol = !!(mode & SPI_CPOL);

	if (speed_hz < OCTEON_SPI_MAX_CLOCK_HZ)
		speed_hz = OCTEON_SPI_MAX_CLOCK_HZ;

	clkdiv = octeon_get_io_clock_rate() / (2 * speed_hz);

	mpi_cfg.u64 = 0;
	mpi_cfg.s.clkdiv = clkdiv;
	mpi_cfg.s.cshi = !!(mode & SPI_CS_HIGH);
	mpi_cfg.s.lsbfirst = !!(mode & SPI_LSB_FIRST);
	mpi_cfg.s.wireor = !!(mode & SPI_3WIRE);
	mpi_cfg.s.idlelo = cpha != cpol;
	mpi_cfg.s.cslate = cpha;
	mpi_cfg.s.enable = 1;

	switch (spi->cs) {
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
		return -1;
	}

	if (mpi_cfg.u64 != cvmx_read_csr(CVMX_MPI_CFG))
		cvmx_write_csr(CVMX_MPI_CFG, mpi_cfg.u64);

	while (len > OCTEON_SPI_MAX_BYTES) {
		if (tx_data)
			for (i = 0; i < OCTEON_SPI_MAX_BYTES; i++) {
				u8 d = *tx_data++;
				cvmx_write_csr(CVMX_MPI_DATX(i), d);
			}

		mpi_tx.u64 = 0;
		mpi_tx.s.csid = slave->cs;
		mpi_tx.s.leavecs = 1;
		mpi_tx.s.txnum = tx_data ? OCTEON_SPI_MAX_BYTES : 0;
		mpi_tx.s.totnum = OCTEON_SPI_MAX_BYTES;
		cvmx_write_csr(CVMX_MPI_TX, mpi_tx.u64);

		octeon_spi_wait_ready();

		if (rx_data)
			for (i = 0; i < OCTEON_SPI_MAX_BYTES; i++) {
				u64 v = cvmx_read_csr(CVMX_MPI_DATX(i));
				*rx_data++ = (u8)v;
			}

		len -= OCTEON_SPI_MAX_BYTES;
	}

	if (tx_data)
		for (i = 0; i < len; i++) {
			u8 d = *tx_data++;
			cvmx_write_csr(CVMX_MPI_DATX(i), d);
		}

	mpi_tx.u64 = 0;
	mpi_tx.s.csid = slave->cs;
	mpi_tx.s.leavecs = !(flags & SPI_XFER_END);
	mpi_tx.s.txnum = tx_data ? len : 0;
	mpi_tx.s.totnum = len;
	cvmx_write_csr(CVMX_MPI_TX, mpi_tx.u64);

	octeon_spi_wait_ready();

	if (rx_data)
		for (i = 0; i < len; i++) {
			u64 v = cvmx_read_csr(CVMX_MPI_DATX(i));
			*rx_data++ = (u8)v;
		}
	return 0;
}

/**
 * Returns if the bus and chip select is valid
 *
 * @param bus	SPI bus number
 * @param cs	Chip select
 *
 * @return 0 if bus or chip select is invalid, otherwise they're valid
 */
static int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return (!bus && (cs >= 0 && cs <= 3));
}

/*********** SPI flash driver support ********************/

/**
 * Place an address into the command
 *
 * @param addr		address
 * @param[out] cmd	Command stream
 */
static void spi_flash_addr(u32 addr, u8 *cmd)
{
	/* cmd[0] is the actual command */
	cmd[1] = addr >> 16;
	cmd[2] = addr >> 8;
	cmd[3] = addr >> 0;
}

/**
 * Perform a write operation followed by a read operation
 * @param 	spi		pointer to spi data structure
 * @param[in]	cmd		command to write
 * @param	cmd_len		Length of command
 * @param[in]	data_out	Data to transmit
 * @param[out]	data_in		Pointer to data to receive
 * @param	data_len	Length od data
 *
 * @return 0 for success, failure otherwise
 */
static int spi_flash_read_write(struct octeon_spi *spi, const u8 *cmd,
				size_t cmd_len, const u8 *data_out,
				u8 *data_in, size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (!ret && data_len != 0)
		ret = spi_xfer(spi, data_len * 8, data_out, data_in,
			       SPI_XFER_END);
	return ret;
}

/**
 * Send a command that expects a response
 * @param	spi		Pointer to spi data structure
 * @param	cmd		Command to send
 * @param[out]	response	Buffer for command response
 * @param	len		Length of expected response
 *
 * @return	0 for success, failure otherwise
 */
int spi_flash_cmd_read(struct octeon_spi *spi, u8 cmd, void *response,
		       size_t len)
{
	return spi_flash_cmd_read(spi, &cmd, 1, response, len);
}

/**
 * Send a command and data
 * @param	spi		Pointer to spi data structure
 * @param[in]	cmd		Command to send
 * @param	cmd_len		Length of command
 * @param[in]	data		Data following command
 * @param	data_len	Length of data
 *
 * @return	0 for success, failure otherwise
 */
static int spi_flash_cmd_write(struct octeon_spi *spi,
			       const u8 *cmd, size_t cmd_len,
			       const void *data, size_t data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, data, NULL, data_len);
}

static int spi_flash_cmd_wait_ready(struct spi_flash *flash,
				    unsigned long timeout)
{
	struct octeon_spi *spi = flash->spi;
	unsigned long timebase;
	int ret;
	u8 status;
	u8 check_status = 0x0;
	u8 poll_bit = STATUS_WIP;
	u8 cmd = flash->poll_cmd;

	if (cmd == CMD_FLAG_STATUS) {
		poll_bit = STATUS_PEC;
		check_status = poll_bit;
	}

	ret = spi_xfer(spi, 8, &cmd, NULL, SPI_XFER_BEGIN);
	if (ret)
		return ret;

	timebase = get_timer(0);
	do {
		ret = spi_xfer(spi, 8, NULL, &status, 0);
		if (ret)
			return -1;

		if ((status & poll_bit) == check_status)
			break;

	} while (get_timer(timebase) < timeout);

	spi_xfer(spi, 0, NULL, NULL, SPI_XFER_END);

	if ((status & poll_bit) == check_status)
		return 0;

	/* Timed out */
	return -1;
}

int spi_flash_read_common(struct spi_flash *flash, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	struct spi_slave *spi = flash->spi;
	int ret;

	ret = spi_claim_bus(flash->spi);
	if (ret)
		return ret;


	ret = spi_flash_cmd_read(spi, cmd, cmd_len, data, data_len);
	if (ret < 0)
		return ret;

	spi_release_bus(spi);

	return ret;
}

#ifdef CONFIG_SPI_FLASH_BAR
int spi_flash_cmd_bankaddr_write(struct spi_flash *flash, u8 bank_sel)
{
	u8 cmd;
	int ret;

	if (flash->bank_curr == bank_sel)
		return 0;

	cmd = flash->bank_write_cmd;
	ret = spi_flash_write_common(flash, &cmd, 1, &bank_sel, 1);
	if (ret < 0)
		return ret;

	flash->bank_curr = bank_sel;

	return 0;
}

int spi_flash_bank_config(struct spi_flash *flash, u8 idcode0)
{
	u8 cmd;
	u8 curr_bank = 0;

	/* discover bank cmds */
	switch (idcode0) {
	case SPI_FLASH_SPANSION_IDCODE0:
		flash->bank_read_cmd = CMD_BANKADDR_BRRD;
		flash->bank_write_cmd = CMD_BANKADDR_BRWR;
		break;
	case SPI_FLASH_STMICRO_IDCODE0:
	case SPI_FLASH_WINBOND_IDCODE0:
		flash->bank_read_cmd = CMD_EXTNADDR_RDEAR;
		flash->bank_write_cmd = CMD_EXTNADDR_WREAR;
		break;
	default:
		return -1;
	}

	/* read the bank reg - on which bank the flash is in currently */
	cmd = flash->bank_read_cmd;
	if (flash->size > SPI_FLASH_16MB_BOUN) {
		if (spi_flash_read_common(flash, &cmd, 1, &curr_bank, 1))
			return -1;

		flash->bank_curr = curr_bank;
	} else {
		flash->bank_curr = curr_bank;
	}

	return 0;
}
#endif

int spi_flash_cmd_read_fast(struct spi_flash *flash, u32 offset,
		size_t len, void *data)
{
	u8 cmd[5], bank_sel = 0;
	u32 remain_len, read_len;
	int ret = -1;

	/* Handle memory-mapped SPI */
	if (flash->memory_map) {
		memcpy(data, flash->memory_map + offset, len);
		return 0;
	}

	cmd[0] = CMD_READ_ARRAY_FAST;
	cmd[4] = 0x00;

	while (len) {
#ifdef CONFIG_SPI_FLASH_BAR
		bank_sel = offset / SPI_FLASH_16MB_BOUN;

		ret = spi_flash_cmd_bankaddr_write(flash, bank_sel);
		if (ret)
			return ret;
#endif
		remain_len = (SPI_FLASH_16MB_BOUN * (bank_sel + 1) - offset);
		if (len < remain_len)
			read_len = len;
		else
			read_len = remain_len;

		spi_flash_addr(offset, cmd);

		ret = spi_flash_read_common(flash, cmd, sizeof(cmd),
							data, read_len);
		if (ret < 0)
			break;

		offset += read_len;
		len -= read_len;
		data += read_len;
	}

	return ret;
}

/**
 * The following table holds all device probe functions
 *
 * @shift:  number of continuation bytes before the ID
 * @idcode: the expected IDCODE or 0xff for non JEDEC devices
 * @probe:  the function to call
 *
 * Non JEDEC devices should be ordered in the table such that
 * the probe functions with best detection algorithms come first.
 *
 * Several matching entries are permitted, they will be tried
 * in sequence until a probe function returns non NULL.
 *
 * IDCODE_CONT_LEN may be redefined if a device needs to declare a
 * larger "shift" value.  IDCODE_PART_LEN generally shouldn't be
 * changed.  This is the max number of bytes probe functions may
 * examine when looking up part-specific identification info.
 *
 * Probe functions will be given the idcode buffer starting at their
 * manu id byte (the "idcode" in the table below).  In other words,
 * all of the continuation bytes will be skipped (the "shift" below).
 */
#define IDCODE_CONT_LEN 0
#define IDCODE_PART_LEN 5
static const struct {
	const u8 shift;
	const u8 idcode;
	struct spi_flash *(*probe) (struct spi_slave *spi, u8 *idcode);
} flashes[] = {
	/* Keep it sorted by define name */
#ifdef CONFIG_SPI_FLASH_ATMEL
	{ 0, 0x1f, spi_flash_probe_atmel, },
#endif
#ifdef CONFIG_SPI_FLASH_EON
	{ 0, 0x1c, spi_flash_probe_eon, },
#endif
#ifdef CONFIG_SPI_FLASH_GIGADEVICE
	{ 0, 0xc8, spi_flash_probe_gigadevice, },
#endif
#ifdef CONFIG_SPI_FLASH_MACRONIX
	{ 0, 0xc2, spi_flash_probe_macronix, },
#endif
#ifdef CONFIG_SPI_FLASH_SPANSION
	{ 0, 0x01, spi_flash_probe_spansion, },
#endif
#ifdef CONFIG_SPI_FLASH_SST
	{ 0, 0xbf, spi_flash_probe_sst, },
#endif
#ifdef CONFIG_SPI_FLASH_STMICRO
	{ 0, 0x20, spi_flash_probe_stmicro, },
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND
	{ 0, 0xef, spi_flash_probe_winbond, },
#endif
#ifdef CONFIG_SPI_FRAM_RAMTRON
	{ 6, 0xc2, spi_fram_probe_ramtron, },
# undef IDCODE_CONT_LEN
# define IDCODE_CONT_LEN 6
#endif
	/* Keep it sorted by best detection */
#ifdef CONFIG_SPI_FLASH_STMICRO
	{ 0, 0xff, spi_flash_probe_stmicro, },
#endif
#ifdef CONFIG_SPI_FRAM_RAMTRON_NON_JEDEC
	{ 0, 0xff, spi_fram_probe_ramtron, },
#endif
};
#define IDCODE_LEN (IDCODE_CONT_LEN + IDCODE_PART_LEN)

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_slave *spi;
	struct spi_flash *flash = NULL;
	int ret, i, shift;
	u8 idcode[IDCODE_LEN], *idp;

	spi = spi_setup_slave(bus, cs, max_hz, spi_mode);
	if (!spi) {
		printf("SF: Failed to set up slave\n");
		return NULL;
	}

	ret = spi_claim_bus(spi);
	if (ret) {
		debug("SF: Failed to claim SPI bus: %d\n", ret);
		goto err_claim_bus;
	}

	/* Read the ID codes */
	ret = spi_flash_cmd(spi, CMD_READ_ID, idcode, sizeof(idcode));
	if (ret)
		goto err_read_id;

#ifdef DEBUG
	printf("SF: Got idcodes\n");
	print_buffer(0, idcode, 1, sizeof(idcode), 0);
#endif

	/* count the number of continuation bytes */
	for (shift = 0, idp = idcode;
	     shift < IDCODE_CONT_LEN && *idp == 0x7f;
	     ++shift, ++idp)
		continue;

	/* search the table for matches in shift and id */
	for (i = 0; i < ARRAY_SIZE(flashes); ++i)
		if (flashes[i].shift == shift && flashes[i].idcode == *idp) {
			/* we have a match, call probe */
			flash = flashes[i].probe(spi, idp);
			if (flash)
				break;
		}

	if (!flash) {
		printf("SF: Unsupported manufacturer %02x\n", *idp);
		goto err_manufacturer_probe;
	}

#ifdef CONFIG_SPI_FLASH_BAR
	/* Configure the BAR - disover bank cmds and read current bank  */
	ret = spi_flash_bank_config(flash, *idp);
	if (ret < 0)
		goto err_manufacturer_probe;
#endif

#ifdef CONFIG_OF_CONTROL
	if (spi_flash_decode_fdt(gd->fdt_blob, flash)) {
		debug("SF: FDT decode error\n");
		goto err_manufacturer_probe;
	}
#endif
	printf("SF: Detected %s with page size ", flash->name);
	print_size(flash->sector_size, ", total ");
	print_size(flash->size, "");
	if (flash->memory_map)
		printf(", mapped at %p", flash->memory_map);
	puts("\n");
#ifndef CONFIG_SPI_FLASH_BAR
	if (flash->size > SPI_FLASH_16MB_BOUN) {
		puts("SF: Warning - Only lower 16MiB accessible,");
		puts(" Full access #define CONFIG_SPI_FLASH_BAR\n");
	}
#endif

	spi_release_bus(spi);

	return flash;

err_manufacturer_probe:
err_read_id:
	spi_release_bus(spi);
err_claim_bus:
	spi_free_slave(spi);
	return NULL;
}

/******************** ATMEL driver *************************/
