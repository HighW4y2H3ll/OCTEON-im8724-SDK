/*-
 * (C) Copyright 2000 Wolfgang Denk, DENX Software Engineering, wd@denx.de
 * Copyright (C) 2003 Bernardo Innocenti <bernie@develer.com>
 *
 * Copyright (c) 2013, Cavium, Inc. <support@cavium.com>
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2 of
 * the License.
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
 *
 * NOTE: This file is GPL and should not be included in the cvmx library nor
 * should it be linked with any proprietary applications.  This file is based
 *  on the U-Boot USB EHCI implementation.  The code is taken from a combination
 * of files, including common/usb.c, common/usb_storage.c, common/usb_hub.c
 *
 * NOTE: This code is designed FOR CHIP VERIFICATION PURPOSES ONLY!  DO NOT USE
 * THIS CODE IN ANY PRODUCT!  YOU HAVE BEEN WARNED!
 *
 * NOTE: This file has been optimized for OCTEON III only!
 */

#ifdef OCX_BOOT
# include <ocx-config.h>
#else
# include <spi-config.h>
#endif
#include <stdlib.h>
#include <ctype.h>
#include "tinylibc.h"
#include "octeon.h"
#include "cvmx-mio-defs.h"
#include "cvmx-rst-defs.h"
#include "cvmx-asm.h"
#include "cvmx-access.h"
#include "cvmx-clock.h"
#include "cvmx-bootloader.h"
#include "cvmx-mio-defs.h"
#include <stdio.h>

extern unsigned long __start, __end;
extern int timer_init(void);

int cvmx_node = 0;
int cvmx_uart = 0;
unsigned long timestamp = 0;
uint64_t cpu_speed = 1000000000ull;

/**
 * This array is used for ctype operations like islower(), etc.
 */
const unsigned char _ctype[] = {
	_C,_C,_C,_C,_C,_C,_C,_C,			/* 0-7 */
	_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,		/* 8-15 */
	_C,_C,_C,_C,_C,_C,_C,_C,			/* 16-23 */
	_C,_C,_C,_C,_C,_C,_C,_C,			/* 24-31 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,			/* 32-39 */
	_P,_P,_P,_P,_P,_P,_P,_P,			/* 40-47 */
	_D,_D,_D,_D,_D,_D,_D,_D,			/* 48-55 */
	_D,_D,_P,_P,_P,_P,_P,_P,			/* 56-63 */
	_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,	/* 64-71 */
	_U,_U,_U,_U,_U,_U,_U,_U,			/* 72-79 */
	_U,_U,_U,_U,_U,_U,_U,_U,			/* 80-87 */
	_U,_U,_U,_P,_P,_P,_P,_P,			/* 88-95 */
	_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,	/* 96-103 */
	_L,_L,_L,_L,_L,_L,_L,_L,			/* 104-111 */
	_L,_L,_L,_L,_L,_L,_L,_L,			/* 112-119 */
	_L,_L,_L,_P,_P,_P,_P,_C,			/* 120-127 */
/** In many cases, ctype for values 128-255 isn't needed and saves 128 bytes */
#ifndef CONFIG_ASCII_CTYPE_LIMIT
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
	_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
	_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
	_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
	_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
	_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L        /* 240-255 */
#endif
};

#define CONFIG_SYS_HZ		(1000000)	/* 1us granularity */
#define CYCLES_PER_JIFFY	((cpu_speed + CONFIG_SYS_HZ / 2) / CONFIG_SYS_HZ)

void tinylibc_init(void)
{
	cvmx_node = 0;
	cvmx_uart = 0;
	timestamp = 0;
}
/**
 * Get clock rate based on the clock type.  Only supports OCTEON III
 *
 * @param node  - CPU node number
 * @param clock - Enumeration of the clock type.
 * @return      - return the clock rate.
 */
uint64_t octeon_clock_get_rate_node(int cvmx_node, cvmx_clock_t clock)
{
	uint64_t rate_eclk = 0;
	uint64_t rate_sclk = 0;

	cvmx_rst_boot_t rst_boot;
	rst_boot.u64 = cvmx_read_csr_node(cvmx_node, CVMX_RST_BOOT);
	rate_eclk = REF_CLOCK * rst_boot.s.c_mul;
	rate_sclk = REF_CLOCK * rst_boot.s.pnr_mul;

	switch (clock) {
	case CVMX_CLOCK_SCLK:
	case CVMX_CLOCK_TIM:
	case CVMX_CLOCK_IPD:
		return rate_sclk;

	case CVMX_CLOCK_RCLK:
	case CVMX_CLOCK_CORE:
		return rate_eclk;
	case CVMX_CLOCK_DDR:
	default:
		break;
	}

	printf("cvmx_clock_get_rate: Unknown clock type %d\n", clock);
	return 0;
}


uint32_t __div64_32(uint64_t *n, uint32_t base)
{
	uint64_t rem = *n;
	uint64_t b = base;
	uint64_t res, d = 1;
	uint32_t high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (uint64_t) high << 32;
		rem -= (uint64_t) (high*base) << 32;
	}

	while ((int64_t)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

lldiv_t lldiv(long long int numer, long long int denom)
{
	lldiv_t retval;

	retval.quot = numer / denom;
	retval.rem = numer % denom;
	if (numer >= 0 && retval.rem < 0) {
		retval.quot++;
		retval.rem -= denom;
	}
	return retval;
}

/**
 * Sets the baudrate of the uart
 *
 * @param baudrate		Baud rate in bits per second
 * @param use_flow_control	!0 to enable hardware flow control
 */
void octeon_set_baudrate(int baudrate, int use_flow_control)
{
	union cvmx_mio_uartx_lcr uart_lcr;
	union cvmx_mio_uartx_mcr uart_mcr;
	union cvmx_mio_uartx_lsr uart_lsr;
	int divisor;

	do {
		uart_lsr.u64 = cvmx_read_csr_node(cvmx_node,
						  CVMX_MIO_UARTX_LSR(cvmx_uart));
	} while (!uart_lsr.s.thre);

	uart_lcr.u64 = cvmx_read_csr_node(cvmx_node,
					  CVMX_MIO_UARTX_LCR(cvmx_uart));
	uart_lcr.s.dlab = 1;		/* Divisor Latch Address bit */
	uart_lcr.s.eps = 0;		/* Even Parity Select */
	uart_lcr.s.pen = 0;		/* Parity Enable */
	uart_lcr.s.stop = 0;		/* Stop control bit */
	uart_lcr.s.cls = 3;		/* Character Length Select */
	cvmx_write_csr_node(cvmx_node, CVMX_MIO_UARTX_LCR(cvmx_uart),
			    uart_lcr.u64);

	cvmx_write_csr_node(cvmx_node, CVMX_MIO_UARTX_FCR(cvmx_uart), 7);

	divisor = octeon_clock_get_rate_node(cvmx_node, CVMX_CLOCK_SCLK)
							/ baudrate / 16;
	cvmx_write_csr_node(cvmx_node, CVMX_MIO_UARTX_DLH(cvmx_uart),
			    divisor >> 8);
	cvmx_write_csr_node(cvmx_node, CVMX_MIO_UARTX_DLL(cvmx_uart),
			    divisor & 0xff);

	uart_lcr.s.dlab = 0;
	cvmx_write_csr_node(cvmx_node, CVMX_MIO_UARTX_LCR(cvmx_uart),
			    uart_lcr.u64);
	cvmx_wait(1000);

	uart_mcr.u64 = cvmx_read_csr_node(cvmx_node,
					  CVMX_MIO_UARTX_MCR(cvmx_uart));
	uart_mcr.s.afce = !!use_flow_control;
	uart_mcr.s.rts = !!use_flow_control;
	cvmx_write_csr_node(cvmx_node, CVMX_MIO_UARTX_MCR(cvmx_uart),
			    uart_mcr.u64);
	mdelay(1);
}

void uart_write_byte(uint8_t b)
{
	union cvmx_mio_uartx_lsr uart_lsr;
	union cvmx_mio_uartx_thr uart_thr;

	do {
		uart_lsr.u64 = cvmx_read_csr_node(cvmx_node,
						  CVMX_MIO_UARTX_LSR(cvmx_uart));
	} while (!uart_lsr.s.thre);
	uart_thr.u64 = 0;
	uart_thr.s.thr = b;
	cvmx_write_csr_node(cvmx_node, CVMX_MIO_UARTX_THR(cvmx_uart),
			    uart_thr.u64);
}

int fputc(int c, FILE *stream __attribute__((unused)))
{
	if (c == '\n')
		uart_write_byte('\r');
	uart_write_byte(c);
	return c;
}

int _IO_putc(int c, _IO_FILE *fp)
{
	return fputc(c, fp);
}

/**
 * Sets the current UART node and UART number
 * @param node		Output node to use
 * @param uart		Output UART to use
 */
void octeon_set_uart(int node, int uart)
{
	cvmx_node = node;
	cvmx_uart = uart;
}

/**
 * Resets the chip
 */
void octeon_do_reset(void)
{
	/* Flush the cache */
	CVMX_SYNC;
	CVMX_ICACHE_INVALIDATE;
	CVMX_DCACHE_INVALIDATE;
	cvmx_write_csr(CVMX_RST_SOFT_RST, 1ull);
}

static inline uint32_t read_c0_count(void)
{
	uint32_t res;
	asm volatile ("mfc0 %0, $9, 0" : "=r"(res));

	return res;
}

static inline uint32_t read_c0_compare(void)
{
	uint32_t res;
	asm volatile ("mfc0 %0, $11, 0" : "=r"(res));

	return res;
}

static inline void write_c0_compare(uint32_t val)
{
	asm volatile ("mtc0 %0, $11, 0" : : "Jr"(val));
}

/**
 * Return the current CPU multiplier
 *
 * @return the current CPU multiplier
 */
static inline int octeon_get_cpu_multiplier(void)
{
	cvmx_mio_rst_boot_t rst_boot;

	rst_boot.u64 = cvmx_read_csr_node(cvmx_get_node_num(), CVMX_RST_BOOT);
	return rst_boot.s.c_mul;
}

/**
 * Returns the CPU speed in hz
 *
 * @return CPU speed in HZ
 */
uint64_t octeon_get_cpu_speed(void)
{
	return octeon_get_cpu_multiplier() * REF_CLOCK;
}

void udelay(uint32_t usec)
{
	unsigned int tmo;
#ifdef OCTEON_SIM
	usec = 1;
#endif
	tmo = read_c0_count() + (usec * (cpu_speed / 1000000));
	while ((tmo - read_c0_count()) < 0x7fffffff)
		/*NOP*/;
}

void mdelay(unsigned long msec)
{
	unsigned long i;
#ifdef OCTEON_SIM
	udelay(1);
#endif
	for (i = 0; i < msec; i++)
		udelay(1000);
}

unsigned int get_timer(unsigned int base)
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

	return timestamp - base;
}

/*
 * Print data buffer in hex and ascii form to the terminal.
 *
 * data reads are buffered so that each memory address is only read once.
 * Useful when displaying the contents of volatile registers.
 *
 * parameters:
 *    addr: Starting address to display at start of line
 *    data: pointer to data buffer
 *    width: data value width.  May be 1, 2, or 4.
 *    count: number of values to display
 *    linelen: Number of values to print per line; specify 0 for default length
 */
#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)
int print_buffer(unsigned long addr, const void* data, unsigned int width,
		 unsigned int count, unsigned int linelen)
{
	unsigned int i;
	unsigned int max_len;
	/* linebuf as a union causes proper alignment */
	union linebuf {
		uint64_t ud[MAX_LINE_LENGTH_BYTES/sizeof(uint64_t) + 1];
		uint32_t ui[MAX_LINE_LENGTH_BYTES/sizeof(uint32_t) + 1];
		uint16_t us[MAX_LINE_LENGTH_BYTES/sizeof(uint16_t) + 1];
		uint8_t  uc[MAX_LINE_LENGTH_BYTES/sizeof(uint8_t) + 1];
	} lb;

	if (linelen*width > MAX_LINE_LENGTH_BYTES)
		linelen = MAX_LINE_LENGTH_BYTES / width;
	if (linelen < 1)
		linelen = DEFAULT_LINE_LENGTH_BYTES / width;

	max_len = linelen;
	while (count) {
		printf("%08lx:", addr);

		/* check for overflow condition */
		if (count < linelen)
			linelen = count;

		/* Copy from memory into linebuf and print hex values */
		for (i = 0; i < linelen; i++) {
			uint64_t x;
			if (width == 8)
				x = lb.ud[i] = *(volatile uint64_t *)data;
			else if (width == 4)
				x = lb.ui[i] = *(volatile uint32_t *)data;
			else if (width == 2)
				x = lb.us[i] = *(volatile uint16_t *)data;
			else
				x = lb.uc[i] = *(volatile uint8_t *)data;
			printf(" %0*lx", width * 2, x);
			data += width;
		}

		if (linelen < max_len)
			printf("%0*s", (max_len - linelen) * (width * 2 + 1),
			       " ");
		/* Print data in ASCII characters */
		for (i = 0; i < linelen * width; i++) {
			if (!isprint(lb.uc[i]) || lb.uc[i] >= 0x80)
				lb.uc[i] = '.';
		}
		lb.uc[i] = '\0';
		printf("    %s\n", lb.uc);

		/* update references */
		addr += linelen * width;
		count -= linelen;
	}

	return 0;
}

/**
 * print sizes as "xxx KiB", "xxx.y KiB", "xxx MiB", "xxx.y MiB",
 * xxx GiB, xxx.y GiB, etc as needed; allow for optional trailing string
 * (like "\n")
 */
void print_size(unsigned long long size, const char *s)
{
	unsigned long m = 0, n;
	unsigned long long f;
	static const char names[] = {'E', 'P', 'T', 'G', 'M', 'K'};
	unsigned long d = 10 * ARRAY_SIZE(names);
	char c = 0;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(names); i++, d -= 10) {
		if (size >> d) {
			c = names[i];
			break;
		}
	}

	if (!c) {
		printf("%llu Bytes%s", size, s);
		return;
	}

	n = size >> d;
	f = size & ((1ULL << d) - 1);

	/* If there's a remainder, deal with it */
	if (f) {
		m = (10ULL * f + (1ULL << (d - 1))) >> d;

		if (m >= 10) {
			m -= 10;
			n += 1;
		}
	}

	printf ("%lu", n);
	if (m) {
		printf (".%ld", m);
	}
	printf (" %ciB%s", c, s);
}

void *memcpy(void *s1, const void *s2, size_t n)
{
	uint8_t *p1 = (uint8_t *)s1;
	const uint8_t *p2 = (uint8_t *)s2;
	while (n--)
		*(p1++) = *(p2++);
	return s1;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	const uint8_t *p1 = (uint8_t *)s1;
	const uint8_t *p2 = (uint8_t *)s2;
	if (!n)
		return 0;
	while (--n && (*p1) == (*p2)) {
		p1++;
		p2++;
	}
	return (int)((*p1) - (*p2));
}

void *memset(void *s, int c, size_t n)
{
	uint8_t *ptr = (uint8_t *)s;
	while (n--)
		*(ptr++) = (uint8_t)c;
	return s;
}

size_t strnlen(const char *s, size_t maxlen)
{
	const char *start = s;
	while (*s && maxlen--)
		s++;
	return s - start;
}

size_t strlen(const char *s)
{
	const char *start = s;
	while (*s)
		s++;
	return s - start;
}

uint64_t octeon_ptr_to_phys(void *ptr)
{
#if 0
	u64 eh_val;
	u64 lo;
	u32 pagemask;
	u64 asid;
	unsigned long offset;
	int index;
#endif
	/* For KSEG0 and KSEG1 we can just map directly */
	switch (CAST64(ptr) & 0xFFFFFFFFE0000000ULL) {
	case 0xFFFFFFFF80000000ULL:	/* KSEG0 */
	case 0xFFFFFFFFA0000000ULL:	/* KSEG1 */
		return (long)ptr & 0x1FFFFFFF;
	}
	/* Check if XKPHYS, if so map directly */
	if ((sizeof(ptr) == 8) &&
	    ((long)ptr & 0xC000000000000000ULL) == 0x8000000000000000ULL)
		return (long)ptr & 0x07FFFFFFFFFFFFFFULL;
#if 0	/* We don't use virtual memory */
	/* Clear low bits of address to properly form entry_hi */
	eh_val = (long)ptr & (~0ull << 13);

	/* Preserve ASID value */
	CVMX_MF_ENTRY_HIGH(asid);
	/* Load the virtual address, preserving the ASID value */
	CVMX_MT_ENTRY_HIGH(eh_val | (asid & 0xFF));
	CVMX_TLBP;			/* Probe the TLB */
	CVMX_MF_TLB_INDEX(index);	/* Get index */

	/* Return error if no entry found */
	if (index < 0)
		return 0xDEADBEEFDEADBEEFULL;
	/* return CAST64(ptr); */

	CVMX_TLBR;			/* Read matching TLB entry */
	CVMX_MF_PAGEMASK(pagemask);	/* Get page size */
	pagemask |= 0x1fff;
	offset = (unsigned long)ptr & pagemask;
	/* Figure out which entry based on offset */
	if (offset <= (pagemask >> 1))
		CVMX_MF_ENTRY_LO_0(lo);
	else
		CVMX_MF_ENTRY_LO_1(lo);

	/* Get physical page address */
	lo = (lo & ~0x3f) << 6;

	return lo + ((unsigned long)ptr & (pagemask >> 1));
#else
	return 0xDEADBEEFDEADBEEFULL;
#endif
}

/**
 * Convert a physical address to a pointer
 *
 * @param	addr	Physical address to convert
 *
 * @return	pointer to physical address in XKPHYS
 */
void *octeon_phys_to_ptr(uint64_t addr)
{
	return (void *)(addr | 0x8000000000000000);
}

/**
 * Implement CRC using Octeon's CRC hardware unit.
 */
uint32_t crc32_no_comp(uint32_t crc, const uint8_t *buf, size_t len)
{
	volatile uint64_t *ptr64;
	volatile const uint8_t *ptr = buf;
	uint32_t poly = 0x04c11db7;	/* Bit reversed standard CRC32 polynomial */
	CVMX_MT_CRC_POLYNOMIAL(poly);
	/* Do endian byte swap on CRC, then use reflect mode of
	 * MT IV to reverse the bits within the bytes.  This results
	 * in a bit-reversed IV.
	 */
	CVMX_ES32(crc, crc);
	CVMX_MT_CRC_IV_REFLECT(crc);
	while ((((ulong)ptr) & 0x7) && len) {
		CVMX_MT_CRC_BYTE_REFLECT(*ptr++);
		len--;
	}
	ptr64 = (uint64_t *)ptr;
	while (len > 8) {
		CVMX_MT_CRC_DWORD_REFLECT(*ptr64++);
		len -= 8;
	}
	ptr = (uint8_t *)ptr64;
	while (len--)
		CVMX_MT_CRC_BYTE_REFLECT(*ptr++);

	CVMX_MF_CRC_IV_REFLECT(crc);

	return crc;
}

/**
 * Calculate the CRC32 checksum
 *
 * @param crc - starting crc
 * @param p - pointer to data to calculate CRC on
 * @param len - number of bytes to calculate CRC on
 *
 * @return crc32 of data
 */
uint32_t crc32(uint32_t crc, const void *p, size_t len)
{
	return crc32_no_comp(crc ^ 0xffffffff, (uint8_t *)p, len) ^ 0xffffffff;
}

/* These functions are used by failsafe bootloader. */
static uint32_t calculate_header_crc(const bootloader_header_t * header)
{
	uint32_t crc;
	uint32_t hcrc_p;

	hcrc_p = offsetof(bootloader_header_t, hcrc);

	crc = crc32(0, (void *)header, hcrc_p);
	hcrc_p += sizeof(header->hcrc);	/* step up pass header crc field */
	crc = crc32(crc, (void *)(((ulong)header) + hcrc_p),
		    header->hlen - hcrc_p);

	return crc;
}

static uint32_t calculate_image_crc(const bootloader_header_t * header)
{
	return crc32(0, (void *)(((ulong)header) + header->hlen),
		     header->dlen);
}

int octeon_validate_header(const bootloader_header_t * header)
{
	debug("%s(%p)\n", __func__, header);
	if (header->magic == BOOTLOADER_HEADER_MAGIC) {
		/* check if header length field valid */
		if (header->hlen > BOOTLOADER_HEADER_MAX_SIZE) {
			printf("Corrupted header length field\n");
			return 1;
		}

		if ((header->maj_rev == 1) && (header->min_rev == 0)) {
			printf("Image header version 1.0, relocation not supported\n");
			return 1;
		}
		/* Check the CRC of the header */
		if (calculate_header_crc(header) == header->hcrc)
			return 0;
		else {
			printf("Header crc check failed\n");
			return 1;
		}
	}

	return 1;
}

/**
 *  Given valid header returns image size (data + header); or 0
 *
 * @param header - pointer to bootloader header
 *
 * @returns size of the image in bytes or 0 if the header is invalid.
 */
uint32_t octeon_get_image_size(const bootloader_header_t * header)
{
	uint32_t isize = 0;
	if (!octeon_validate_header(header))
		/* failsafe has valid header - get the image size */
		isize = header->hlen + header->dlen;

	return isize;
}

/**
 * Validate image data.
 * Not NAND or NOR specific
 *
 * @param header     Address of expected image header.
 *
 * @return 0  on success
 *         1  on failure
 */
int octeon_validate_data(const bootloader_header_t * header)
{
	uint32_t image_size, crc;

	if ((image_size = octeon_get_image_size(header)) > 0x200000)
		return 1;

	crc = calculate_image_crc(header);
	return (crc != header->dcrc);
}

static int octeon_validate_bootloader_header(const bootloader_header_t *header)
{
	if (header->magic == BOOTLOADER_HEADER_MAGIC) {
		/* Check the CRC of the header */
		if (calculate_header_crc(header) == header->hcrc)
			return 1;
		else
			return 0;
	}
	return 0;
}

/**
 * This function searches the NOR flash for a normal bootloader and returns a
 * pointer to the beginning of it if found or NULL if not found.
 *
 * @returns pointer to bootloader in NOR flash or NULL if not found.
 */
bootloader_header_t *octeon_find_and_validate_normal_bootloader(void)
{
	bootloader_header_t *header = (bootloader_header_t *)0xffffffffbfc00000ULL;
	uint8_t *scan_ptr, *scan_end;
	ulong our_size;

	if (!octeon_validate_bootloader_header(header))
		return NULL;

	our_size = &__end - &__start;

	scan_ptr = (void *)header;
	scan_ptr += ((our_size + 0xffff) & 0xffff0000);
	/* Search first 4MB */
	scan_end = (uint8_t *)0xffffffffbfff0000ull;

	for (; scan_ptr <= scan_end; scan_ptr += 0x10000) {
		header = (bootloader_header_t *)scan_ptr;
		if (octeon_validate_bootloader_header(header)) {
			if (header->dcrc ==
			    crc32(0, scan_ptr + header->hlen, header->dlen))
				return header;
		}
	}
	return NULL;
}

/**
 * Searches for a valid bootloader following the ATE tool in the NOR flash.
 * If found it branches to the next bootloader.
 * If not found then 0 is returned.
 * NOTE: This resets the NOR flash start address on the boot bus to 0x1fc00000.
 */
int octeon_bootloader_scan(void)
{
	bootloader_header_t *header;
	cvmx_mio_boot_reg_cfgx_t reg_cfg;

	reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));
	reg_cfg.s.base = 0x1fc0;
	cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(0), reg_cfg.u64);


	header = octeon_find_and_validate_normal_bootloader();
	if (header) {
		printf("Jumping to start of image at address 0x%p\n", header);
		asm volatile (
			".set	push			\n"
			".set	noreorder		\n"
			"sync				\n"
			"cache	0, 0($0)		\n"
			"nop				\n"
			"cache	1, 0($0)		\n"
			"nop				\n"
			"j	%[addr]			\n"
			"cache	0, 0($0)		\n"
			".set	pop			\n"
			: : [addr] "r"(header) : "memory"
		);
	}
	return 0;
}

/**
 * Lock a single line into L2. The line is zeroed before locking
 * to make sure no dram accesses are made.
 *
 * @param addr   Physical address to lock
 */
void l2c_lock_line(uint64_t addr)
{
	addr |= (2ull << 62);
	asm("cache 31, 0(%[line])" :: [line] "r" (addr));	/* Lock L2 */
}

/**
 * Locks a memory region in the L2 cache
 *
 * @param start - start address to begin locking
 * @param len - length in bytes to lock
 */
void l2c_lock_mem_region(uint64_t start, uint64_t len)
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
void l2c_unlock_line(uint64_t addr)
{
	char *addr_ptr = octeon_phys_to_ptr(addr);

	asm("cache 23, %[line]	\n"	/* Unlock the line */
		:: [line] "m" (*addr_ptr));
}

/**
 * Unlock a memory region in the L2 cache
 *
 * @param start - start address to unlock
 * @param len - length to unlock in bytes
 */
void l2c_unlock_mem_region(uint64_t start, uint64_t len)
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
