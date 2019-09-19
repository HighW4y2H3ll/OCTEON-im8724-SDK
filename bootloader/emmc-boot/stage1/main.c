/*
 * Copyright 2012 Cavium, Inc. <support@cavium.com>
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

/* Stage 1 eMMC bootloader
 *
 * NOTE: The final binary MUST be no more than 8192 bytes in size!
 *
 * This file is not linked with any other file except start.S in order
 * to help the compiler better optimize the code for size.
 */

#include "config.h"
#include <stdint.h>
#include "cvmx.h"
#include "cvmx-mio-defs.h"
#include "cvmx-ipd-defs.h"
#include "cvmx-l2c-defs.h"
#include "cvmx-clock.h"
#include "cvmx-asm.h"
#include "cvmx-bootloader.h"
#include "cvmx-l2c.h"
#include "mmc.h"
#include "uart.h"
#include "fat.h"
#include "partition.h"
#if defined(GPIO_FAILSAFE)
#include "cvmx-gpio.h"
#endif

/**
 * OCTEON cache line size
 */
#define CACHE_LINE_SIZE		128
#define COP0_PRID		$15,0
#define OCTEON_CN70XX_PRID	0x96
#define OCTEON_CN78XX_PRID	0x95
#define OCTEON_CNF71XX_PRID	0x94
#define OCTEON_CN61XX_PRID	0x93

/* convenient macros */
#define min(X, Y)				\
	({	typeof(X) __x = (X);		\
		typeof(Y) __y = (Y);		\
		(__x < __y) ? __x : __y; })

#define max(X, Y)				\
	({	typeof(X) __x = (X);		\
		typeof(Y) __y = (Y);		\
		(__x > __y) ? __x : __y; })

typedef void (*move_and_start_t)(uint64_t dest, uint64_t src_start,
				 uint64_t src_end);
/**
 * Partition table in start.S
 */
extern struct partition partition_table[4] __attribute__((aligned(2)));

/**
 * Branches to the destination address
 *
 * @param dst_addr - Address to branch to
 */
extern void jump_to_bootloader(uint64_t dst_addr) __attribute__((noreturn));

/**
 * Moves the bootloader to a lower address and starts it
 */
extern void move_and_start(uint64_t dest, uint64_t src_start, uint64_t size)
	__attribute__((noreturn));
extern char move_and_start_end;

/**
 * Covert a physical address into a C pointer
 *
 * @param address Address to convert
 *
 * @return Pointer value
 */
static inline void *phys_to_ptr(uint64_t address)
{
    return (void*)(address | (1ull<<63));
}

/**
 * Covert a C pointer into a physical address
 *
 * @param ptr    Pointer to convert
 *
 * @return Physical address
 */
static inline uint64_t ptr_to_phys(void *ptr)
{
    return 0x7fffffffull & (unsigned long)ptr;
}

/**
 * Writes end of line to the serial port
 */
static void uart_write_eol(void)
{
	uart_write_string("\r\n");
}

/**
 * Writes a string followed by CR, LF to the serial port
 *
 * @param s - string to write
 */
static void uart_write_stringl(const char *s)
{
	uart_write_string(s);
	uart_write_eol();
}

/**
 * Writes a 64-bit hex number out the serial port followed by CR, LF
 *
 * @param val - value to write
 */
static void uart_write_hexl(uint64_t val)
{
	uart_write_hex(val);
	uart_write_eol();
}

/**
 * Replacement for the CVMX version that doesn't require including
 * a bunch of stuff.
 *
 * @param address
 *
 * @return
 */
static inline uint64_t __cvmx_read_csr(uint64_t address)
{
    return *(volatile uint64_t*)address;
}


/**
 * Replacement for the CVMX version that doesn't require including
 * a bunch of stuff.
 *
 * @param address
 *
 * @return
 */
static inline void __cvmx_write_csr(uint64_t address, uint64_t value)
{
    *(volatile uint64_t*)address = value;
}

static inline int get_octeon_model(void)
{
	uint32_t id;
	asm("mfc0	%0, $15,0" : "=r"(id));
	return (id >> 8) & 0xff;
}

static inline int octeon_is_octeon3(uint8_t model)
{
	return (model >= OCTEON_CN78XX_PRID);
}

static inline uint8_t l2c_get_set_bits(uint8_t model)
{
				/*         777776666 */
				/*         530811683 */
	static const uint64_t set_bits = 0xBBAD99ABA;
	return (set_bits >> (model - 0x90) * 4) & 0xF;
}

static inline int octeon_is_cn70xx(void)
{
	return get_octeon_model() == OCTEON_CN70XX_PRID;
}

/**
 * Lock a single line into L2. The line is zeroed before locking
 * to make sure no dram accesses are made.
 *
 * @param addr   Physical address to lock
 */
static void l2c_lock_line(uint64_t addr)
{
	addr |= (2ull << 62);
	asm("cache 31, 0(%[line])" :: [line] "r" (addr));	/* Lock L2 */
}

#if defined(VERBOSE_OUTPUT)
static void uart_write_dec(uint32_t value)
{
	uint64_t divisor = 10000000;
	int not_zero = 0;
	int digit;

	if (value == 0) {
		uart_write_char('0');
		return;
	}

	do {
		digit = (value % divisor) / (divisor / 10);
		if (digit > 0)
			not_zero = 1;
		if (not_zero)
			uart_write_char(digit + '0');
		divisor /= 10;
	} while (divisor >= 10);
}
#endif

/**
 * Locks a memory region in the L2 cache
 *
 * @param start - start address to begin locking
 * @param len - length in bytes to lock
 */
static void l2c_lock_mem_region(uint64_t start, uint64_t len)
{
	uint64_t end;

	/* Round start/end to cache line boundaries */
	len += start & (CACHE_LINE_SIZE - 1);
	start &= ~(CACHE_LINE_SIZE - 1);
	end = start + len;

	while (start <= end) {
		l2c_lock_line(start);
		start += CACHE_LINE_SIZE;
	}
	asm volatile("sync");
}

/**
 * Unlock a single line in the L2 cache.
 *
 * @param addr	Physical address to unlock
 *
 * @return Zero on success
 */
static void l2c_unlock_line(uint64_t addr)
{
	char *addr_ptr = phys_to_ptr(addr);

	asm("cache 23, %[line]	\n"	/* Unlock the line */
		:: [line] "m" (*addr_ptr));
}

/**
 * Unlock a memory region in the L2 cache
 *
 * @param start - start address to unlock
 * @param len - length to unlock in bytes
 */
static void l2c_unlock_mem_region(uint64_t start, uint64_t len)
{
	uint64_t end;

	/* Round start/end to cache line boundaries */
	len += start & (CACHE_LINE_SIZE - 1);
	start &= ~(CACHE_LINE_SIZE - 1);
	end = start + len;

	while (start <= end) {
		l2c_unlock_line(start);
		start += CACHE_LINE_SIZE;
	}
}

/**
 * Set a buffer of bytes to a value
 *
 * @param s - pointer to buffer to initialize
 * @param c - value to initialize buffer to
 * @param n - number of bytes to set
 *
 * @return pointer to buffer
 */
void *memset(void *s, int c, size_t n)
{
	uint8_t *val = (uint8_t *)s;
	while (n--)
		*val++ = c;
	return s;
}

void *memcpy(void *s1, const void *s2, size_t n)
{
	uint8_t *dest = (uint8_t *)s1;
	const uint8_t *src = (uint8_t *)s2;
	const uint8_t *end = src + n;

	while (src != end)
		*dest++ = *src++;

	return s1;
}

/**
 * Converts a little-endian 32-bit value to the native CPU format
 *
 * @param value - value to convert
 *
 * @return byte-swapped value
 */
static inline uint32_t le32_to_cpu(uint32_t value)
{
	return __builtin_bswap32(value);
}

/**
 * Converts a little-endian 16-bit value to the native CPU format
 *
 * @param value - value to convert
 *
 * @return byte swapped value
 */
static inline uint16_t le16_to_cpu(uint16_t value)
{
	asm(
	"	wsbh	%0, %1		\n"
	: "=r"(value)
	: "r"(value));
	return value;
}

#ifdef STAGE1_VALIDATE_STAGE2_CRC
/**
 * Internal CRC32 calculation taking advantage of the OCTEON CRC32 engine
 *
 * @param crc - initial CRC value
 * @param buf - pointer to buffer to calculate CRC on
 * @param len - length of buffer in bytes
 *
 * @return CRC32 value of buffer
 */
static uint32_t crc32_no_comp(uint32_t crc, const uint8_t *buf, uint32_t len)
{
	volatile uint64_t *ptr64;
	volatile uint8_t *ptr = (void *)buf;
	/* Bit reversed standard CRC32 polynomial - 0xedb88320 */
	uint32_t poly = 0x04c11db7;
	CVMX_MT_CRC_POLYNOMIAL(poly);
	/* Do endian byte swap on CRC, then use reflect mode of
	 * MT IV to reverse the bits within the bytes.  This results
	 * in a bit-reversed IV. */
	CVMX_ES32(crc, crc);
	CVMX_MT_CRC_IV_REFLECT(crc);
	while ((((intptr_t)ptr) & 0x7) && len) {
		CVMX_MT_CRC_BYTE_REFLECT(*ptr++);
		len--;
	}
	ptr64 = (void *)ptr;
	while (len > 8) {
		CVMX_MT_CRC_DWORD_REFLECT(*ptr64++);
		len -= 8;
	}
	ptr = (void *)ptr64;
	while (len--)
		CVMX_MT_CRC_BYTE_REFLECT(*ptr++);

	CVMX_MF_CRC_IV_REFLECT(crc);

	return crc;
}

/**
 * Perform a CRC32 calculation on a buffer
 *
 * @param crc - initial crc value
 * @param p - pointer to buffer
 * @param len - length of buffer
 *
 * @return CRC value
 */
static uint32_t crc32(uint32_t crc, const uint8_t *p, uint32_t len)
{
	return crc32_no_comp(crc ^ 0xffffffffL, p, len) ^ 0xffffffffL;
}

/**
 * Calculates the CRC over the stage 2 bootloader header while skipping the
 * CRC32 value embedded in the header
 *
 * @param header - bootloader header
 *
 * @return CRC32 value
 */
static uint32_t calculate_header_crc(const bootloader_header_t * header)
{
	uint32_t crc;
	uint32_t hcrc_p;

	hcrc_p = offsetof(bootloader_header_t, hcrc);

	crc = crc32(0, (void *)header, hcrc_p);
	hcrc_p += sizeof(header->hcrc);	/* step up pass header crc field */
	crc =
	    crc32(crc, (void *)(((uint64_t) header) + hcrc_p),
		  header->hlen - hcrc_p);

	return crc;
}

/**
 * Validates the header of the STAGE 2 bootloader
 *
 * @param header - pointer to stage 2 bootloader header
 *
 * @return 0 for success
 */
static int validate_header(const bootloader_header_t *header)
{
	if (header->magic != BOOTLOADER_HEADER_MAGIC)
		return 1;
	if (header->hlen > BOOTLOADER_HEADER_MAX_SIZE)
		/* Corrupt header length */
		return 1;
	if ((header->maj_rev == 1) && (header->min_rev == 0))
		/* Relocation not supported */
		return 1;
	if (calculate_header_crc(header) != header->hcrc)
		return 1;

	return 0;
}

/**
 * Returns the size of the stage 2 bootloader based on what is encoded in the
 * header.
 *
 * @param header - pointer to stage 2 bootloader header
 *
 * @return size of stage 2 bootloader including the header in bytes or 0 if
 *         error.
 */
static uint32_t get_image_size(const bootloader_header_t *header)
{
	uint32_t isize = 0;
	if (!validate_header(header))
		isize = header->hlen + header->dlen;
	return isize;
}
#endif /* STAGE1_VALIDATE_STAGE2_CRC */

#ifdef DEBUG
/**
 * Prints out a buffer of 512 bytes
 *
 * @param sector - pointer to sector data
 */
static void print_sector(uint32_t sector_num, const void *sector, uint32_t count)
{
	uint64_t *ptr = (uint64_t *)sector;
	int i;
	uart_write_string("Sector: ");
	uart_write_hex(sector_num);
	for (i = 0; i < (FAT_SECTOR_SIZE / 8) * count; i++) {
		if ((i % FAT_SECTOR_SIZE) == 0)
			uart_write_eol();
		if (i % 2 == 0) {
			uart_write_eol();
			uart_write_hex(i*8);
			uart_write_char(':');
		}
		uart_write_char(' ');
		uart_write_hex(*ptr++);
	}
	uart_write_eol();
}
#endif

static inline int failsafe_mode(void)
{
#ifdef GPIO_FAILSAFE
	cvmx_gpio_rx_dat_t gpio_rx_dat;

	gpio_rx_dat.u64 = __cvmx_read_csr(CVMX_GPIO_RX_DAT);
	if (GPIO_FAILSAFE_LEVEL)
		return ((gpio_rx_dat.s.dat >> GPIO_FAILSAFE) & 1);
	else
		return !((gpio_rx_dat.s.dat >> GPIO_FAILSAFE) & 1);
#else
	return 0;
#endif /* GPIO_FAILSAFE */
}

/**
 * Returns the I/O clock frequency in hz
 *
 * @return frequency in hz
 */
static uint64_t get_io_clock(void)
{
	int mult;
	if (octeon_is_octeon3(get_octeon_model())) {
		cvmx_rst_boot_t rst_boot;
		rst_boot.u64 = __cvmx_read_csr(CVMX_RST_BOOT);
		mult = rst_boot.s.c_mul;
	} else {
		cvmx_mio_rst_boot_t mio_rst_boot;
		mio_rst_boot.u64 = __cvmx_read_csr(CVMX_MIO_RST_BOOT);
 		mult = mio_rst_boot.s.c_mul;
	}
	return REF_CLOCK * mult;
}

/**
 * Sends a command to the MMC device
 *
 * @param cmd_idx - command index
 * @param arg - argument to pass to command
 * @param flags - various flags to pass, needed for some SD commands to override
 *		  the default OCTEON values for MMC.
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_cmd(int cmd_idx, uint32_t arg, uint32_t flags)
{
	cvmx_mio_emm_cmd_t	emm_cmd;
	cvmx_mio_emm_rsp_sts_t	emm_rsp_sts;

	emm_cmd.u64 = 0;
	emm_cmd.s.cmd_val = 1;
	emm_cmd.s.cmd_idx = cmd_idx;
	emm_cmd.s.arg = arg;
	emm_cmd.s.ctype_xor = (flags >> 3) & 3;
	emm_cmd.s.rtype_xor = (flags & 7);
	emm_cmd.s.offset = (flags >> 8) & 0x3f;
	__cvmx_write_csr(CVMX_MIO_EMM_CMD, emm_cmd.u64);

	do {
		emm_rsp_sts.u64 = __cvmx_read_csr(CVMX_MIO_EMM_RSP_STS);
	} while (!emm_rsp_sts.s.cmd_done && !emm_rsp_sts.s.rsp_timeout);
	if (emm_rsp_sts.s.rsp_timeout) {
#if 0
		uart_write_string("sendcmd: ");
		uart_write_hex((uint64_t)cmd_idx<<32 | arg);
		uart_write_stringl(" timeout");
#endif
		return 2;
	}
	else if (emm_rsp_sts.s.rsp_bad_sts || emm_rsp_sts.s.rsp_crc_err)
	{
		uart_write_string("sendcmd: ");
		uart_write_hex((uint64_t)cmd_idx<<32 | arg);
		uart_write_stringl(" bad status or crc");
		return 1;
	}
	return 0;
}

/**
 * Sends an ACMD to a SD device.
 *
 * @param cmd_idx - command index to send
 * @param arg - command argument
 * @param flags - flags to use (see mmc_send_cmd)
 * @param rca - RCA address of SD device
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_acmd(int cmd_idx, uint32_t arg, uint32_t flags, uint16_t rca)
{
	int rc;

	rc = mmc_send_cmd(MMC_CMD_APP_CMD, rca << 16, 0);
	if (rc)
		return rc;
	return mmc_send_cmd(cmd_idx, arg, flags);
}

/**
 * Sends IDLE commands to force the MMC or SD device to go idle
 *
 * @return 0 for success, error otherwise
 */
static int mmc_go_idle(void)
{
	mmc_send_cmd(0, 0, 0);
	return mmc_send_cmd(0, 0, 0);
}

/**
 * Reads the MMC OCR register
 *
 * @param mmc - pointer to MMC data structure
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_op_cond(struct mmc *mmc)
{
	cvmx_mio_emm_rsp_lo_t emm_rsp_lo;
	int rc;
	int timeout = 1000;

	mmc_go_idle();
	cvmx_wait(10000);

	do {
		rc = mmc_send_cmd(MMC_CMD_SEND_OP_COND, 0x40ff8000, 0);
		emm_rsp_lo.u64 = __cvmx_read_csr(CVMX_MIO_EMM_RSP_LO);
		cvmx_wait(1000);
	} while ((rc == 0) && !(emm_rsp_lo.u64 & (1ull << (31 + 8))) && (timeout-- > 0));
	if (rc || !timeout)
		return 1;
	mmc->ocr = (emm_rsp_lo.u64 >> 8) & 0xffffffff;
	if (mmc->ocr & 0x40000000)
		mmc->flags |= MMC_FLAG_HIGH_CAPACITY;
	return 0;
}

/**
 * Get the extended CSD register from the MMC/SD card
 *
 * @param mmc - MMC data structure
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_ext_csd(struct mmc *mmc)
{
	cvmx_mio_emm_buf_idx_t emm_buf_idx;
	cvmx_mio_emm_buf_dat_t emm_buf_dat;
	int rc;
	int i;

	rc = mmc_send_cmd(MMC_CMD_SEND_EXT_CSD, 0, 0);
	if (rc) {
		uart_write_stringl("EXT CSD failed!");
		return rc;
	}
	emm_buf_idx.u64 = 0;
	emm_buf_idx.s.inc = 1;
	__cvmx_write_csr(CVMX_MIO_EMM_BUF_IDX, emm_buf_idx.u64);
	for (i = 0; i < (int)sizeof(mmc->ext_csd.buffer)/8; i++) {
		emm_buf_dat.u64 = __cvmx_read_csr(CVMX_MIO_EMM_BUF_DAT);
		mmc->ext_csd.buffer[i] = emm_buf_dat.u64;
	}
	return 0;
}

static int mmc_calc_clock_period(struct mmc *mmc)
{
	return (((mmc->sclock + mmc->clock - 1) / mmc->clock) + 1) / 2;
}
/**
 * Configures the MMC bus
 *
 * @param mmc - pointer to MMC data structure
 *
 * @return 0 for success, otherwise error
 */
static int mmc_set_ios(struct mmc *mmc)
{
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_rsp_sts_t emm_sts;
	int clk_period;
	int timeout = 2550;

	emm_switch.u64 = 0;
	emm_switch.s.power_class = 10;

again:
	switch (mmc->bus_width) {
	case 8:
		if (mmc->flags & MMC_FLAG_HIGH_SPEED_DDR)
			emm_switch.s.bus_width = EXT_CSD_DDR_BUS_WIDTH_8;
		else
			emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_8;
		break;
	case 4:
		if (mmc->flags & MMC_FLAG_HIGH_SPEED_DDR)
			emm_switch.s.bus_width = EXT_CSD_DDR_BUS_WIDTH_4;
		else
			emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_4;
		break;
	default:
		emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_1;
		break;
	}
	clk_period = mmc_calc_clock_period(mmc);
	emm_switch.s.switch_exe = 1;
	emm_switch.s.clk_hi = clk_period;
	emm_switch.s.clk_lo = emm_switch.s.clk_hi;
	emm_switch.s.hs_timing =
		(mmc->flags & (MMC_FLAG_HIGH_SPEED | MMC_FLAG_HIGH_SPEED_52MHz))
		? 1 : 0;
	__cvmx_write_csr(CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	do {
		emm_sts.u64 = __cvmx_read_csr(CVMX_MIO_EMM_RSP_STS);
		if (!emm_sts.s.switch_val)
			break;
		cvmx_wait(10000);
	} while (timeout-- > 0);
	if (emm_switch.s.switch_err0) {
		if (emm_switch.s.power_class > 0) {
			emm_switch.s.power_class--;
			goto again;
		} else {
			uart_write_stringl("Power class failed");
			return 1;
		}
	}
	if (emm_switch.s.switch_err1) {
		if (mmc->flags & MMC_FLAG_HIGH_SPEED_DDR) {
			mmc->flags &= ~MMC_FLAG_HIGH_SPEED_DDR;
			goto again;
		}
		if ((mmc->flags & MMC_FLAG_HIGH_SPEED_52MHz) &&
		    (mmc->clock > 26000000)) {
			mmc->flags &= ~MMC_FLAG_HIGH_SPEED_52MHz;
			mmc->flags |= MMC_FLAG_HIGH_SPEED;
			mmc->clock = 26000000;
			goto again;
		}
		if (mmc->flags & MMC_FLAG_HIGH_SPEED) {
			if (MMC_MAX_SPEED < 20000000)
				mmc->clock =  MMC_MAX_SPEED;
			else
				mmc->clock = 20000000;
			mmc->flags &= ~MMC_FLAG_HIGH_SPEED;
			goto again;
		}
		uart_write_stringl("MMC timing failed");
		return 2;
	}
	if (emm_switch.s.switch_err2) {
		switch (emm_switch.s.bus_width) {
		case EXT_CSD_BUS_WIDTH_8:
			emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_4;
			break;
		case EXT_CSD_BUS_WIDTH_4:
			emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_1;
			break;
		default:
			uart_write_stringl("Bus width failed");
			return 3;
		}
		goto again;
	}
	return 0;
}

/**
 * Initialize SD bus settings
 *
 * @param mmc - pointer to MMC data structure
 *
 * @return 0 for success
 */
static int sd_set_ios(struct mmc *mmc)
{
	cvmx_mio_emm_switch_t emm_switch;
	int clk_period;

	clk_period = mmc_calc_clock_period(mmc);
	emm_switch.u64 = 0;
	/* No DDR for now */
	emm_switch.s.bus_width = (mmc->bus_width == 4) ?
				EXT_CSD_BUS_WIDTH_4 : EXT_CSD_BUS_WIDTH_1;
	emm_switch.s.clk_hi = clk_period;
	emm_switch.s.clk_lo = emm_switch.s.clk_hi;
	emm_switch.s.power_class = 10;
	__cvmx_write_csr(CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	cvmx_wait(20000);
	return 0;
}

/**
 * Reads one or more sectors from a MMC or SD device
 *
 * @param mmc - pointer to MMC data structure
 * @param src - starting sector number
 * @param dst - pointer to destination buffer
 * @param count - number of sectors to read
 *
 * @return 0 for success, error otherwise
 */
static int mmc_read(struct mmc *mmc, uint32_t src, void *dst, int count)
{
	cvmx_mio_emm_buf_idx_t buf_idx;
	uint64_t *dst_ptr = (uint64_t *)dst;
	int i;
	int rc;

	if (!(mmc->flags & MMC_FLAG_HIGH_CAPACITY))
		src *= FAT_SECTOR_SIZE;

	while (count-- > 0) {
		rc = mmc_send_cmd(MMC_CMD_READ_SINGLE_BLOCK, src, 0);
		if (rc) {
			uart_write_stringl("Read failed");
			return rc;
		}
		buf_idx.u64 = 0;
		buf_idx.s.inc = 1;
		__cvmx_write_csr(CVMX_MIO_EMM_BUF_IDX, buf_idx.u64);
		for (i = 0; i < FAT_SECTOR_SIZE / 8; i++) {
			*dst_ptr++ = __cvmx_read_csr(CVMX_MIO_EMM_BUF_DAT);
		}
		if (mmc->flags & MMC_FLAG_HIGH_CAPACITY)
			src++;
		else
			src += FAT_SECTOR_SIZE;
	}
	return 0;
}

/**
 * Mounts a FAT filesystem and initializes the fat information structure
 *
 * @param mmc - pointer to mmc data structure
 * @param finfo[out] - pointer to FAT information structure to be initialized
 * @param start_sector - sector where FAT partition begins
 *
 * @return 0 for success, error otherwise
 */
static int fat_mount(struct mmc *mmc, struct fat_info *finfo, uint32_t start_sector)
{
	struct fat_boot_sector bsec;

	if (mmc_read(mmc, start_sector, &bsec, 1) != 0) {
		return -1;
	}

	/* Validate the signature */
	if (bsec.signature != FAT_BOOT_SIGNATURE) {
		uart_write_string("Bad FAT signature ");
		uart_write_hexl(bsec.signature);
		return -1;
	}

	finfo->start_sector = start_sector;
	finfo->sector_size = le16_to_cpu(bsec.sector_size);
	finfo->cluster_size = bsec.sec_per_clus;
	finfo->fat_start_sector = le16_to_cpu(bsec.reserved) + start_sector;
	finfo->fat_length = le16_to_cpu(bsec.fat_length);
	finfo->root_size = le16_to_cpu(bsec.dir_entries);
	finfo->total_sector_count = le32_to_cpu(bsec.total_sect);
	finfo->root_dir_sectors =
			(finfo->root_size * sizeof(struct fat_dir_entry) +
			 finfo->sector_size - 1) / finfo->sector_size;

	if (bsec.fat32_length && !bsec.fat_length && finfo->cluster_size != 0) {
		/* FAT32 */
		finfo->fat_type = FAT_TYPE_FAT32;
		finfo->fat_length = le32_to_cpu(bsec.fat32_length);
		finfo->root_cluster = le32_to_cpu(bsec.root_cluster);
	} else {
		/* FAT12/FAT16 */
		if (finfo->cluster_size == 0)
			finfo->cluster_size = 4;
		finfo->fat_type = FAT_TYPE_FAT16;
		finfo->root_cluster = 0;
		finfo->fat_length = le16_to_cpu(bsec.fat_length);
	}

	finfo->num_fats = bsec.fats;
	finfo->cluster_start_sector =
		finfo->fat_start_sector + finfo->fat_length * finfo->num_fats;
	finfo->current_fat_table = 0;
	return 0;
}

/**
 * Maps a cluster to a sector number
 *
 * @param finfo - pointer to fat information data structure
 * @param cluster - cluster number to convert
 *
 * @return 32-bit sector number relative to the start of the device
 */
static uint32_t fat_cluster_to_sector(const struct fat_info *finfo,
				      uint32_t cluster)
{
	uint32_t sector;

	if (finfo->fat_type == FAT_TYPE_FAT16)
		cluster -= 2;
	sector = finfo->cluster_size * cluster +
			finfo->cluster_start_sector + finfo->root_dir_sectors;
	return sector;
}

/**
 * Returns the next cluster number for a given cluster
 *
 * @param mmc - pointer to MMC data structure
 * @param finfo - pointer to fat information structure
 * @param cluster - cluster to look up
 * @param[out] next_cluster - next cluster in the list
 *
 * @return 0 for success, error otherwise
 */
static int fat_get_next_cluster(struct mmc *mmc, struct fat_info *finfo,
				uint32_t cluster, uint32_t *next_cluster)
{
	uint32_t fat_sector;
	uint32_t next;

	cluster += finfo->root_cluster;

	if (finfo->fat_type == FAT_TYPE_FAT32) {
		fat_sector = finfo->fat_start_sector + cluster / 128;
	} else {
		fat_sector = finfo->fat_start_sector + cluster / 256;
	}
	/* Adjust for selected FAT table */
	fat_sector += finfo->current_fat_table * finfo->fat_length;

	if (fat_sector != finfo->last_fat_sector) {
		if (mmc_read(mmc, fat_sector, &finfo->fat_cache.fat32[0], 1) != 0) {
			uart_write_stringl("Error reading FAT");
			return -1;
		}
		finfo->last_fat_sector = fat_sector;
	}

	switch (finfo->fat_type) {
	case FAT_TYPE_FAT32:
		next = le32_to_cpu(finfo->fat_cache.fat32[cluster % 128]);
		if ((next >= 0x0FFFFFF8) && (next <= 0x0FFFFFFF)) /* EOF */
			return 1;
		else if (next > 0x0FFFFFFF)
			return -1;
		break;
	case FAT_TYPE_FAT16:
		next = le16_to_cpu(finfo->fat_cache.fat16[cluster % 256]);
		if (next >= 0xfff8)
			return 1;
		break;
	default:
		return 1;
	}
	*next_cluster = next - finfo->root_cluster;
	return 0;
}

/**
 * Given a cluster number and a sector offset this function will output the
 * next cluster number and sector offset.
 *
 * @param mmc - pointer to mmc data structure
 * @param finfo - pointer to FAT information data structure
 * @param[in,out] cluster - current cluster number, outputs next cluster number
 * @param[in,out] sector_offset - sector offset for the cluster
 *
 * @return 0 for success, 1 if end of chain, < 0 on error
 */
static int fat_get_next_sector(struct mmc *mmc, struct fat_info *finfo,
			       uint32_t *cluster, uint32_t *sector_offset)
{
	uint32_t next_cluster;
	int rc;

	if (finfo->fat_type == FAT_TYPE_FAT16 && (*cluster) == 0) {
		if ((*sector_offset) < finfo->root_dir_sectors) {
			(*sector_offset)++;
			return 0;
		}
		return 1;
	} else {
		if ((*sector_offset) + 1 < finfo->cluster_size) {
			(*sector_offset)++;
		} else {
			(*sector_offset) = 0;
			rc = fat_get_next_cluster(mmc, finfo, *cluster,
						  &next_cluster);
			if (!rc)
				(*cluster) = next_cluster;
			return rc;
		}
	}
	return 0;
}

/**
 * Locates a file in the root directory
 *
 * @param mmc - pointer to mmc data structure
 * @param finfo - pointer to FAT information structure
 * @param match - information on the match (if found) as well as the filename
 *		  to search for.  Note that the filename must be 11 characters
 *		  in length.  If the main part of a 8.3 filename is shorter
 *		  than 8 characters then it must be padded with spaces.  Do not
 * 		  include the '.' character and pad with spaces at the end too.
 * 		  If successful, it will contain information about the file
 * 		  such as the starting cluster number and file size in bytes.
 *
 * @return 0 for match found, 1 if no match found, error if less than zero
 */
static int fat_find_file(struct mmc *mmc, struct fat_info *finfo,
			 struct fat_dir_info *match)
{
	struct fat_dir_entry dir_entries[FAT_SECTOR_SIZE/sizeof(struct fat_dir_entry)];
	uint32_t cluster = 0;
	uint32_t sector;
	uint32_t sector_offset = 0;
	int dir_index = 0;
	int k;
	int found = 0;
	int rc;

	do {
		/* Go through each sector in the cluster one by one */
		sector = finfo->cluster_start_sector +
				cluster * finfo->cluster_size + sector_offset;

		if (mmc_read(mmc, sector, &dir_entries[0], 1) != 0)
			return -1;

		/* Go through each entry in the sector, there should be 16 */
		for (dir_index = 0;
		     dir_index < (int)(FAT_SECTOR_SIZE/sizeof(struct fat_dir_entry)) && !found;
		     dir_index++) {
			if (dir_entries[dir_index].shortname[0] == '\0')
				return 1;/* Not found, end of list */

			/* Make sure it's not a directory, volume label
			 * or other type of file we don't care about.
			 * The volume label flag will also catch the
			 * extended VFAT filenames.
			 */
			if (dir_entries[dir_index].attributes &
			    (FAT_ATTR_VOLUME_LABEL |
			     FAT_ATTR_DIRECTORY |
			     FAT_ATTR_DEVICE))
				continue;

			found = 1;
			for (k = 0; k < 11; k++) {
				if (match->shortname[k] !=
				    dir_entries[dir_index].shortname[k]) {
					found = 0;
					break;
				}
			}
			if (found) {
				goto found_match;
			}
		}
		/* Go on to the next cluster */
		rc = fat_get_next_sector(mmc, finfo, &cluster, &sector_offset);
		if (rc < 0)
			return -2;
		else if (rc > 0)
			return 1;
	} while (!found);
	return 0;

found_match:
	match->file_size = le32_to_cpu(dir_entries[dir_index].file_size);
	match->attributes = dir_entries[dir_index].attributes;
	match->start_cluster = le16_to_cpu(dir_entries[dir_index].start_first_cluster);
	if (finfo->fat_type == FAT_TYPE_FAT32)
		match->start_cluster |= (le16_to_cpu(dir_entries[dir_index].high_first_cluster) << 16);
	match->start_cluster -= finfo->root_cluster;
	return 0;
}

/**
 * Given a starting cluster number and size, read a file into a buffer at
 * load_addr
 *
 * @param mmc - pointer to mmc data structure
 * @param finfo - pointer to FAT information structure
 * @param start_cluster - starting cluster number of file
 * @param load_addr - address of buffer where file should be loaded
 *
 * @return 0 if successful, 1 if premature EOF, -1 if read error
 */
static int fat_read_file(struct mmc *mmc, struct fat_info *finfo,
			 struct fat_dir_info *dir_info,
			 uint64_t load_addr)
{
	uint32_t sector;
	uint32_t current_cluster = dir_info->start_cluster;
	uint32_t next_cluster;
	uint32_t read_size;
	int rc;
	int size_left = dir_info->file_size;

	while (size_left > 0) {
		sector = fat_cluster_to_sector(finfo, current_cluster);
		if (size_left >= (finfo->cluster_size * finfo->sector_size))
			read_size = finfo->cluster_size;
		else
			read_size = (size_left + finfo->sector_size - 1) / finfo->sector_size;
		if (mmc_read(mmc, sector, (void *)load_addr, read_size) != 0)
			return -1;
#ifdef DEBUG
		print_sector(sector, (uint8_t *)load_addr, read_size);
#endif
		uart_write_char('.');
		size_left -= read_size * finfo->sector_size;
		if (size_left <= 0)
			break;
		load_addr += finfo->cluster_size * finfo->sector_size;
		rc = fat_get_next_cluster(mmc, finfo, current_cluster,
					  &next_cluster);
		if (rc < 0)
			return rc;
		else if (rc > 0 && dir_info->file_size >= finfo->cluster_size) {
			uart_write_stringl("\r\nPremature EOF");
			return 1;
		}
		current_cluster = next_cluster;
	}

	uart_write_stringl(" Done.");

	return 0;
}

/**
 * Validates a bootloader by checking that both the header and data CRCs are
 * correct.
 *
 * @param ptr - pointer to where the bootloader is located in memory
 *
 * @return 0 if bootloader is good, 1 if error
 */
static int validate_bootloader(void *ptr)
{
	bootloader_header_t *header = (bootloader_header_t *)ptr;
	uint32_t crc;

	if (validate_header(header)) {
		uart_write_stringl("Header invalid.");
		return 1;
	}

	crc = crc32(0, (uint8_t *)header + header->hlen, header->dlen);
	if (header->dcrc == crc) {
		return 0;
	} else {
		uart_write_string("CRC doesn't match header.  CRC: ");
		uart_write_hex(crc);
		uart_write_string(", expected CRC: ");
		uart_write_hexl(header->dcrc);
		return 1;
	}
}

/**
 * The core of the code is located here.
 */
void main(void)
{
	struct mmc mmc;
	struct fat_info finfo;
	struct fat_dir_info dir_info;
	cvmx_mio_emm_cfg_t emm_cfg;
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_wdog_t emm_wdog;
	cvmx_mio_emm_rsp_lo_t emm_rsp_lo;
#if defined(STAGE1_RESET_MMC)
	cvmx_mio_emm_rsp_sts_t rsp_sts;
	int timeout;
#endif
	const char *filename;
	uint32_t freq, mult;
	uint32_t size;
	int retries = 0;
	int i;
	int rc;
	bootloader_header_t *header;
	uint64_t start_addr;
	move_and_start_t fn_ptr;
	uint64_t fn_size;
	int image_size;

	/* frequency bases */
	/* divided by 10 to be nice to platforms without floating point */
	static const int fbase[] = {
		10000,
		100000,
		1000000,
		10000000,
	};

	/* Multiplier values for TRAN_SPEED.  Multiplied by 10 to be nice
	* to platforms without floating point.
	*/
	static const uint8_t multipliers[] = {
		0,	/* reserved */
		10, 12, 13, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80, };

	l2c_lock_mem_region(STAGE1_DRAM_ADDR, STAGE1_BOOTLOADER_SIZE);

	if (failsafe_mode())
		goto error;
#if defined(STAGE2_ENV_FILENAME)
	l2c_lock_mem_region(RAM_ENV_ADDR, 2 * U_BOOT_RAM_ENV_SIZE);
#endif
	/* Re-enable L2 ways */
	__cvmx_write_csr(CVMX_L2C_WPAR_PPX(0), 0);

	memset(&mmc, 0, sizeof(struct mmc));
	memset(&finfo, 0, sizeof(finfo));

	mmc.sclock = get_io_clock();
retry_mmc:
	mmc.clock = MMC_MIN_SPEED;
	mmc.version = MMC_VERSION_UNKNOWN;

	/* Reset MMC bus */
	emm_cfg.u64 = 0;
	__cvmx_write_csr(CVMX_MIO_EMM_CFG, emm_cfg.u64);
	cvmx_wait(10000);
	/* Enable MMC */
	emm_cfg.s.bus_ena = 1;
	__cvmx_write_csr(CVMX_MIO_EMM_CFG, emm_cfg.u64);

	emm_wdog.u64 = 0;
	emm_wdog.s.clk_cnt = 256000 + (8 * MMC_MIN_SPEED) / 10;
	__cvmx_write_csr(CVMX_MIO_EMM_WDOG, emm_wdog.u64);

	/* Configure power class 10, speed 400KHz */
	emm_switch.u64 = 0;
	emm_switch.s.power_class = 10;
	emm_switch.s.clk_hi = ((mmc.sclock / mmc.clock) + 1) / 2;
	emm_switch.s.clk_lo = emm_switch.s.clk_hi;
	__cvmx_write_csr(CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	cvmx_wait(2000);	/* Wait 1ms + 74 clock cycles */

	/* Force MMC device to go idle */
	mmc_go_idle();
	/* Check if SD version 2.0 or later */
	rc = mmc_send_cmd(SD_CMD_SEND_IF_COND, 0x1aa, (1 << 3) | 2);
	emm_rsp_lo.u64 = 0;	/* Make compiler happy */
	if (rc == 0) {
		emm_rsp_lo.u64 = __cvmx_read_csr(CVMX_MIO_EMM_RSP_LO);
		if ((emm_rsp_lo.u64 & 0xff00) == 0xaa00)
			mmc.version = SD_VERSION_2;
	}

	if (IS_SD(&mmc)) {
		int timeout = 1000;
		do {
			uint32_t args = 0xff8000;
			if (mmc.version == SD_VERSION_2)
				args |= 0x40000000;
			rc = mmc_send_acmd(SD_CMD_APP_SEND_OP_COND, args, 3, 0);
			if (rc)
				break;
			emm_rsp_lo.u64 = __cvmx_read_csr(CVMX_MIO_EMM_RSP_LO);
			cvmx_wait(1000);
		} while (!(emm_rsp_lo.u64 & (1ull << (31 + 8))) && (timeout-- > 0));
		if (rc == 0 && timeout > 0) {
			if (mmc.version != SD_VERSION_2)
				mmc.version = SD_VERSION_1_0;
			mmc.ocr = emm_rsp_lo.u64 >> 8;
			if (mmc.ocr & 0x40000000)
				mmc.flags |= MMC_FLAG_HIGH_CAPACITY;
		} else {
			rc = mmc_send_op_cond(&mmc);
		}
	} else {
		rc = mmc_send_op_cond(&mmc);
	}
	rc = mmc_send_cmd(MMC_CMD_ALL_SEND_CID, 0, 0);
	mmc.cid.u128[0] = __cvmx_read_csr(CVMX_MIO_EMM_RSP_HI);
	mmc.cid.u128[1] = __cvmx_read_csr(CVMX_MIO_EMM_RSP_LO);

	if (IS_SD(&mmc)) {
		rc = mmc_send_cmd(SD_CMD_SEND_RELATIVE_ADDR, 0, 2);
		if (!rc) {
			mmc.rca = (__cvmx_read_csr(CVMX_MIO_EMM_RSP_LO) >> 24)
				   & 0xffff;
		}
	} else {
		mmc.rca = 1;
		rc = mmc_send_cmd(MMC_CMD_SET_RELATIVE_ADDR, mmc.rca << 16, 0);
	}

	rc = mmc_send_cmd(MMC_CMD_SEND_CSD, mmc.rca << 16, 0);
	if (rc)
		goto error2;

	mmc.csd.u128[0] = __cvmx_read_csr(CVMX_MIO_EMM_RSP_HI);
	mmc.csd.u128[1] = __cvmx_read_csr(CVMX_MIO_EMM_RSP_LO);
	if (mmc.version == MMC_VERSION_UNKNOWN) {
		switch (mmc.csd.s.spec_vers) {
		default:
		case 0:
			mmc.version = MMC_VERSION_1_2;
			break;
		case 1:
			mmc.version = MMC_VERSION_1_4;
			break;
		case 2:
			mmc.version = MMC_VERSION_2_2;
			break;
		case 3:
			mmc.version = MMC_VERSION_3;
			break;
		case 4:
			mmc.version = MMC_VERSION_4;
			break;
		}
	}
	freq = fbase[mmc.csd.s.tran_speed & 7];
	mult = multipliers[(mmc.csd.s.tran_speed >> 3) & 0xf];
	mmc.tran_speed = freq * mult;
	if (mmc.tran_speed > MMC_MAX_SPEED)
		mmc.tran_speed = MMC_MAX_SPEED;
	mmc.read_bl_len = 1 << mmc.csd.s.read_bl_len;
	if (IS_SD(&mmc))
		mmc.write_bl_len = mmc.read_bl_len;
	else
		mmc.write_bl_len = mmc.csd.s.write_bl_len;

	if (mmc.flags & MMC_FLAG_HIGH_CAPACITY)
		mmc.capacity = ((uint64_t)mmc.csd.s.c_size + 1) << 10;
	else
		mmc.capacity = ((uint64_t)mmc.csd.s.c_size + 1)
					<< (mmc.csd.s.c_size_mult + 2);
	mmc.capacity *= mmc.read_bl_len;
	if (mmc.read_bl_len > FAT_SECTOR_SIZE)
		mmc.read_bl_len = FAT_SECTOR_SIZE;
	if (mmc.write_bl_len > FAT_SECTOR_SIZE)
		mmc.write_bl_len = FAT_SECTOR_SIZE;

	/* Select the MMC device */
	rc = mmc_send_cmd(MMC_CMD_SELECT_CARD, mmc.rca << 16, 0);
	if (rc)
		goto error2;

	if (!IS_SD(&mmc)) {
		if (mmc.bus_width == 0)
			mmc.bus_width = MMC_MAX_WIDTH;
		mmc.clock = 20000000;

		if (mmc.version >= MMC_VERSION_4) {
			rc = mmc_send_ext_csd(&mmc);
			if (rc)
				goto error2;
			if ((mmc.ext_csd.s.ext_csd_rev >= 2)
			    && (mmc.flags & MMC_FLAG_HIGH_CAPACITY))
				mmc.capacity = le32_to_cpu(mmc.ext_csd.s.sec_count) * FAT_SECTOR_SIZE;
			if ((MMC_MAX_SPEED > 20000000) && (mmc.ext_csd.s.device_type & EXT_CSD_DEVICE_TYPE_26MHz)) {
				mmc.flags |= MMC_FLAG_HIGH_SPEED;
				mmc.clock = 26000000;
			}
			if ((MMC_MAX_SPEED > 26000000) && (mmc.ext_csd.s.device_type & EXT_CSD_DEVICE_TYPE_52MHz)) {
				mmc.flags |= MMC_FLAG_HIGH_SPEED_52MHz;
				mmc.clock = 52000000;
			}
			if ((MMC_MAX_SPEED > 26000000) && (mmc.ext_csd.s.device_type & EXT_CSD_DEVICE_TYPE_DDR_52MHz_3V)) {
				mmc.flags |= MMC_FLAG_HIGH_SPEED_DDR;
				mmc.clock = 52000000;
			}
		}
		if (mmc.clock > MMC_MAX_SPEED)
			mmc.clock = MMC_MAX_SPEED;
		rc = mmc_set_ios(&mmc);
		if (rc) {
			uart_write_stringl("mmc_set_ios failed");
		}
		cvmx_wait(10000);
		/* mmc_set_bus_width_speed(&mmc) */
	} else {
		mmc.bus_width = min(MMC_MAX_WIDTH, 4);
		if (MMC_MAX_SPEED < 25000000)
			mmc.clock = MMC_MAX_SPEED;
		else
			mmc.clock = 25000000;
		rc = mmc_send_acmd(SD_CMD_APP_SET_BUS_WIDTH,
				   mmc.bus_width == 4 ? 2 : 0, 0, mmc.rca);
		rc = sd_set_ios(&mmc);
		/* SD change frequency */
		/* sd set bus width speed */
	}

#ifdef MMC_SET_BLOCKLEN
	if (!mmc.flags & MMC_FLAG_HIGH_SPEED_DDR)
		mmc_send_cmd(MMC_CMD_SET_BLOCKLEN, FAT_SECTOR_SIZE, 0);
#endif

	/* Set watchdog timeout again */
	emm_wdog.s.clk_cnt = 256000 + (10 * mmc.clock) / 8;
	__cvmx_write_csr(CVMX_MIO_EMM_WDOG, emm_wdog.u64);

	for (i = 0; i < 4; i++) {
		if (partition_table[i].status == 0x80 &&
		    ((partition_table[i].type == 0x4) ||	/* FAT16 < 32M */
		     (partition_table[i].type == 0x6) ||	/* FAT16 */
		     (partition_table[i].type == 0x14) ||	/* FAT16 < 32M */
		     (partition_table[i].type == 0x16) ||	/* FAT16 */
		     (partition_table[i].type == 0xb) ||	/* FAT32 */
		     (partition_table[i].type == 0xc) ||	/* LBA FAT32 */
		     (partition_table[i].type == 0xe) ||	/* LBA FAT16 */
		     (partition_table[i].type == 0x1b) ||	/* FAT32 */
		     (partition_table[i].type == 0x1c) ||	/* LBA FAT32 */
		     (partition_table[i].type == 0x1e)))	/* LBA FAT16 */
			break;
	}
	if (i == 4)
		goto error;
#if defined(VERBOSE_OUTPUT)
	uart_write_string("\r\nPartition: ");
	uart_write_char(i + '1');
	uart_write_string(", start: ");
	uart_write_hex(le32_to_cpu(partition_table[i].start_lba));
	uart_write_string(", size: ");
	uart_write_hexl(le32_to_cpu(partition_table[i].num_sectors));
#endif
	rc = fat_mount(&mmc, &finfo, le32_to_cpu(partition_table[0].start_lba));
	if (rc) {
		uart_write_stringl("Error mounting FAT partition");
		if (mmc.bus_width == 8) {
			mmc.bus_width = 4;
			goto retry_mmc;
		} else if (mmc.bus_width == 4) {
			mmc.bus_width = 1;
			goto retry_mmc;
		}
		goto error;
	}

again:
	memset(&dir_info, 0, sizeof(dir_info));
	if ((retries & 1) == 0) {
		filename = STAGE_2_FILENAME;
	} else {
		filename = STAGE_2_BACKUP_FILENAME;
	}
	/* If we tried loading both filenames, try again using the alternate FAT
	 * table(s).
	 */
	if (retries & 7) {
		finfo.current_fat_table++;
		if (finfo.current_fat_table >= finfo.num_fats)
			finfo.current_fat_table = 0;
	}
	if (retries > MAX_FAT_RETRIES)
		goto error;

	retries++;

	for (i = 0; i < 11; i++)
		dir_info.shortname[i] = filename[i];

	rc = fat_find_file(&mmc, &finfo, &dir_info);
	if (rc == 1) {
		uart_write_string(filename);
		uart_write_stringl(" not found");
		goto again;
	} else if (rc != 0) {
		uart_write_stringl("DIR error");
		goto again;
	}
#if defined(VERBOSE_OUTPUT)
	uart_write_string("Reading ");
	uart_write_dec(dir_info.file_size);
	uart_write_stringl(" bytes.");
#endif
	/* Lock the memory region where the bootloader is to be loaded */
	l2c_lock_mem_region(STAGE2_DRAM_LOAD_ADDR, dir_info.file_size);

	rc = fat_read_file(&mmc, &finfo, &dir_info, STAGE2_DRAM_LOAD_ADDR);

	if (rc != 0) {
		uart_write_stringl("Error loading stage 2 bootloader");
		goto again;
	}

	uart_write_string("Loaded ");
	uart_write_string(filename);
	uart_write_string(", size: ");
	uart_write_hexl(dir_info.file_size);
#ifdef STAGE1_VALIDATE_STAGE2_CRC
	if (validate_bootloader((void *)STAGE2_DRAM_LOAD_ADDR)) {
		uart_write_stringl("\r\nStage 2 bootloader corrupt.");
		goto again;
	}
#endif

start_stage2:
	/* We have successfully loaded a valid bootloader */

#if defined(STAGE2_ENV_FILENAME)
	if ((retries & 1) == 1) {
		uart_write_string("Loading environment");
		filename = STAGE2_ENV_FILENAME;
		for (i = 0; i < 11; i++)
			dir_info.shortname[i] = filename[i];

		rc = fat_find_file(&mmc, &finfo, &dir_info);

		if (rc == 0)
			rc = fat_read_file(&mmc, &finfo, &dir_info, RAM_ENV_ADDR);

		if ((rc != 0)
#if defined(VALIDATE_ENV_CRC)
		    || (*((uint32_t *)RAM_ENV_ADDR) !=
		     crc32(0,
			   (uint8_t *)(RAM_ENV_ADDR + U_BOOT_RAM_ENV_CRC_SIZE),
			   U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE))
#endif
		   )
			uart_write_stringl(" Error");
		else
			uart_write_stringl(" Done");
	}
#endif

#if defined(STAGE1_RESET_MMC)
	emm_switch.s.switch_exe = 1;
	emm_switch.s.hs_timing = 0;
	emm_switch.s.bus_width = 0;
	emm_switch.s.power_class = 10;
	emm_switch.s.clk_hi = (mmc.sclock / MMC_MIN_SPEED) / 2;
	emm_switch.s.clk_lo = emm_switch.s.clk_hi;
	__cvmx_write_csr(CVMX_MIO_EMM_SWITCH, emm_switch.u64);

	timeout = 2550;
	do {
		rsp_sts.u64 = __cvmx_read_csr(CVMX_MIO_EMM_RSP_STS);
		if (!rsp_sts.s.switch_val)
			break;
		cvmx_wait(10000);
	} while (timeout-- > 0);
	if (!timeout) {
		uart_write_stringl("Reset timed out");
	}
	cvmx_wait(2000);

	/* Disable MMC */
	mmc_go_idle();
	cvmx_wait(1000);

#endif

	/* Disable all EMMC interfaces */
	emm_cfg.u64 = 0;
	emm_cfg.s.bus_ena = 0;
	__cvmx_write_csr(CVMX_MIO_EMM_CFG, emm_cfg.u64);


	header = (bootloader_header_t *)STAGE2_DRAM_LOAD_ADDR;

	start_addr = (STAGE2_DRAM_LOAD_ADDR & ~0x3fffffULL);
	start_addr |= (header->address & 0x3fffff);

	image_size = header->hlen + header->dlen;

	fn_size = (void *)(&move_and_start_end) - (void *)(&move_and_start);
	fn_ptr = (move_and_start_t)
		((uint8_t *)((STAGE2_DRAM_LOAD_ADDR + image_size + 64) & ~7ull));
	memcpy(fn_ptr, &move_and_start, fn_size);

	uart_write_string("Branching to stage 2 at: ");
	uart_write_hexl(start_addr);
	fn_ptr(start_addr, STAGE2_DRAM_LOAD_ADDR, image_size);
#if 0
	/* Unlock our memory space in the L2 cache */
	l2c_unlock_mem_region(STAGE1_DRAM_ADDR, STAGE1_BOOTLOADER_SIZE);

	jump_to_bootloader(STAGE2_START_ADDR);
#endif
error:
#ifdef STAGE_2_LAST_FAILSAFE

	/* If we get here then we have failed to load a stage 2 bootloader from
	 * a FAT partition.  In this case see if there's a valid bootloader
	 * immediately following the stage 1 bootloader.
	 */
	uart_write_stringl("\r\nError: Trying embedded failsafe...");

	/* Lock room for one sector */
	l2c_lock_mem_region(STAGE2_DRAM_LOAD_ADDR, FAT_SECTOR_SIZE);
	rc = mmc_read(&mmc, STAGE_2_LAST_FAILSAFE_START_SECTOR,
		      (void *)STAGE2_DRAM_LOAD_ADDR, 1);
	if (rc)
		goto error2;

	size = get_image_size((bootloader_header_t *)STAGE2_DRAM_LOAD_ADDR);
	if (size == 0)
	{
		if (mmc.bus_width == 8) {
			mmc.bus_width = 4;
			goto retry_mmc;
		} else if (mmc.bus_width == 4) {
			mmc.bus_width = 1;
			goto retry_mmc;
		}
		goto error2;
	}

	/* Lock the rest of the memory in L2 */
	l2c_lock_mem_region(STAGE2_DRAM_LOAD_ADDR + FAT_SECTOR_SIZE,
			    size - FAT_SECTOR_SIZE);

	/* Read the rest of the bootloader */
	rc = mmc_read(&mmc, STAGE_2_LAST_FAILSAFE_START_SECTOR + 1,
		      (uint8_t *)STAGE2_DRAM_LOAD_ADDR + FAT_SECTOR_SIZE,
		      ((size + FAT_SECTOR_SIZE - 1) / FAT_SECTOR_SIZE));
	if (rc)
		goto error2;

	/* Validate that this bootloader is good */
	if (!validate_bootloader((void *)STAGE2_DRAM_LOAD_ADDR)) {
		/* We have a good bootloader, proceed to starting it */
		uart_write_stringl("Success.");
		goto start_stage2;
	}

#endif
error2:
	/* If we get here there's nothing we can do but hang. */
	uart_write_stringl("\r\nNo valid stage 2 bootloader found.");
	emm_cfg.u64 = 0;
	emm_cfg.s.boot_fail = 1;
	__cvmx_write_csr(CVMX_MIO_EMM_CFG, emm_cfg.u64);

	while (1)
		asm volatile ("wait");
	/* We should never return */
}
