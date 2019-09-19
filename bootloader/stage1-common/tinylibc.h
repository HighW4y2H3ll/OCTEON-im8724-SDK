/*
 * (C) Copyright 2013 Cavium, Inc. <support@cavium.com>
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

#ifndef __TINYLIBC_H__
#define __TINYLIBC_H__

#include <linux/types.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "cvmx-abi.h"

#ifdef DEBUG
# define _DEBUG	1
#else
# define _DEBUG 0
#endif

/*
 * Output a debug text when condition "cond" is met. The "cond" should be
 * computed by a preprocessor in the best case, allowing for the best
 * optimization.
 */
#define debug_cond(cond, fmt, args...)	\
	do { if (cond) printf(fmt, ##args); } while (0)

#define debug(fmt, args...)		debug_cond(_DEBUG, fmt, ##args)

#define min(X, Y)	({ typeof(X) __x = (X); typeof(Y) __y = (Y); \
			 (__x < __y) ? __x : __y; })

#define max(X, Y)	({ typeof(X) __x = (X); typeof(Y) __y = (Y); \
			 (__x > __y) ? __x : __y; })

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define ROUND(a,b)		(((a) + (b) - 1) & ~((b) - 1))
#define DIV_ROUND(n,d)		(((n) + ((d)/2)) / (d))
#define DIV_ROUND_UP(n,d)	(((n) + (d) - 1) / (d))
#define roundup(x, y)		((((x) + ((y) - 1)) / (y)) * (y))

#define ALIGN(x,a)		__ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)	(((x)+(mask))&~(mask))

#define uswap_16(x) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
	((((x) & 0xff00000000000000##sfx) >> 56) | \
	 (((x) & 0x00ff000000000000##sfx) >> 40) | \
	 (((x) & 0x0000ff0000000000##sfx) >> 24) | \
	 (((x) & 0x000000ff00000000##sfx) >>  8) | \
	 (((x) & 0x00000000ff000000##sfx) <<  8) | \
	 (((x) & 0x0000000000ff0000##sfx) << 24) | \
	 (((x) & 0x000000000000ff00##sfx) << 40) | \
	 (((x) & 0x00000000000000ff##sfx) << 56))
#if defined(__GNUC__)
# define uswap_64(x) _uswap_64(x, ull)
#else
# define uswap_64(x) _uswap_64(x, )
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define cpu_to_le16(x)		(x)
# define cpu_to_le32(x)		(x)
# define cpu_to_le64(x)		(x)
# define le16_to_cpu(x)		(x)
# define le32_to_cpu(x)		(x)
# define le64_to_cpu(x)		(x)
# define cpu_to_be16(x)		uswap_16(x)
# define cpu_to_be32(x)		uswap_32(x)
# define cpu_to_be64(x)		uswap_64(x)
# define be16_to_cpu(x)		uswap_16(x)
# define be32_to_cpu(x)		uswap_32(x)
# define be64_to_cpu(x)		uswap_64(x)
#else
# define cpu_to_le16(x)		uswap_16(x)
# define cpu_to_le32(x)		uswap_32(x)
# define cpu_to_le64(x)		uswap_64(x)
# define le16_to_cpu(x)		uswap_16(x)
# define le32_to_cpu(x)		uswap_32(x)
# define le64_to_cpu(x)		uswap_64(x)
# define cpu_to_be16(x)		(x)
# define cpu_to_be32(x)		(x)
# define cpu_to_be64(x)		(x)
# define be16_to_cpu(x)		(x)
# define be32_to_cpu(x)		(x)
# define be64_to_cpu(x)		(x)
#endif

#define CACHE_LINE_SIZE		128

typedef unsigned long int ulong;
typedef unsigned char uchar;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef unsigned int uint;

/* stdout */
int printf(const char *fmt, ...)
			__attribute__ ((format (__printf__, 1, 2)));
int vprintf(const char *fmt, va_list args);

ulong simple_strtoul(const char *cp, char **endp, unsigned int base);

/**
 * strict_strtoul - convert a string to an unsigned long strictly
 * @param cp	The string to be converted
 * @param base	The number base to use
 * @param res	The converted result value
 * @return 0 if conversion is successful and *res is set to the converted
 * value, otherwise it returns -EINVAL and *res is set to 0.
 *
 * strict_strtoul converts a string to an unsigned long only if the
 * string is really an unsigned long string, any string containing
 * any invalid char at the tail will be rejected and -EINVAL is returned,
 * only a newline char at the tail is acceptible because people generally
 * change a module parameter in the following way:
 *
 *      echo 1024 > /sys/module/e1000/parameters/copybreak
 *
 * echo will append a newline to the tail.
 *
 * simple_strtoul just ignores the successive invalid characters and
 * return the converted value of prefix part of the string.
 *
 * Copied this function from Linux 2.6.38 commit ID:
 * 521cb40b0c44418a4fd36dc633f575813d59a43d
 *
 */
int strict_strtoul(const char *cp, unsigned int base, unsigned long *res);
unsigned long long simple_strtoull(const char *cp, char **endp,
					unsigned int base);
long simple_strtol(const char *cp, char **endp, unsigned int base);
void panic(const char *fmt, ...)
		__attribute__ ((format (__printf__, 1, 2), noreturn));

/**
 * Format a string and place it in a buffer
 *
 * @param buf	The buffer to place the result into
 * @param fmt	The format string to use
 * @param ...	Arguments for the format string
 *
 * The function returns the number of characters written
 * into @buf.
 *
 * See the vsprintf() documentation for format string extensions over C99.
 */
int sprintf(char *buf, const char *fmt, ...)
		__attribute__ ((format (__printf__, 2, 3)));

/**
 * Format a string and place it in a buffer (va_list version)
 *
 * @param buf	The buffer to place the result into
 * @param size	The size of the buffer, including the trailing null space
 * @param fmt	The format string to use
 * @param args	Arguments for the format string
 * @return the number of characters which have been written into
 * the @buf not including the trailing '\0'. If @size is == 0 the function
 * returns 0.
 *
 * If you're not already dealing with a va_list consider using scnprintf().
 *
 * See the vsprintf() documentation for format string extensions over C99.
 */
int vsprintf(char *buf, const char *fmt, va_list args);
char *simple_itoa(ulong i);

#ifdef CONFIG_SYS_VSNPRINTF
/**
 * Format a string and place it in a buffer
 *
 * @param buf	The buffer to place the result into
 * @param size	The size of the buffer, including the trailing null space
 * @param fmt	The format string to use
 * @param ...	Arguments for the format string
 * @return the number of characters which would be
 * generated for the given input, excluding the trailing null,
 * as per ISO C99.  If the return is greater than or equal to
 * @size, the resulting string is truncated.
 *
 * See the vsprintf() documentation for format string extensions over C99.
 */
int snprintf(char *buf, size_t size, const char *fmt, ...)
		__attribute__ ((format (__printf__, 3, 4)));

/**
 * Format a string and place it in a buffer
 *
 * @param buf	The buffer to place the result into
 * @param size	The size of the buffer, including the trailing null space
 * @param fmt	The format string to use
 * @param ...	Arguments for the format string
 *
 * The return value is the number of characters written into @buf not including
 * the trailing '\0'. If @size is == 0 the function returns 0.
 *
 * See the vsprintf() documentation for format string extensions over C99.
 */
int scnprintf(char *buf, size_t size, const char *fmt, ...)
		__attribute__ ((format (__printf__, 3, 4)));

/**
 * Format a string and place it in a buffer (base function)
 *
 * @param buf	The buffer to place the result into
 * @param size	The size of the buffer, including the trailing null space
 * @param fmt	The format string to use
 * @param args	Arguments for the format string
 * @return The number characters which would be generated for the given
 * input, excluding the trailing '\0', as per ISO C99. Note that fewer
 * characters may be written if this number of characters is >= size.
 *
 * This function follows C99 vsnprintf, but has some extensions:
 * %pS output the name of a text symbol
 * %pF output the name of a function pointer
 * %pR output the address range in a struct resource
 *
 * The function returns the number of characters which would be
 * generated for the given input, excluding the trailing '\0',
 * as per ISO C99.
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want snprintf() instead.
 */
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);

/**
 * Format a string and place it in a buffer (va_list version)
 *
 * @param buf	The buffer to place the result into
 * @param size	The size of the buffer, including the trailing null space
 * @param fmt	The format string to use
 * @param args	Arguments for the format string
 * @return the number of characters which have been written into
 * the @buf not including the trailing '\0'. If @size is == 0 the function
 * returns 0.
 *
 * If you're not already dealing with a va_list consider using scnprintf().
 *
 * See the vsprintf() documentation for format string extensions over C99.
 */
int vscnprintf(char *buf, size_t size, const char *fmt, va_list args);
#else
/*
 * Use macros to silently drop the size parameter. Note that the 'cn'
 * versions are the same as the 'n' versions since the functions assume
 * there is always enough buffer space when !CONFIG_SYS_VSNPRINTF
 */
#define snprintf(buf, size, fmt, args...) sprintf(buf, fmt, ##args)
#define scnprintf(buf, size, fmt, args...) sprintf(buf, fmt, ##args)
#define vsnprintf(buf, size, fmt, args...) vsprintf(buf, fmt, ##args)
#define vscnprintf(buf, size, fmt, args...) vsprintf(buf, fmt, ##args)
#endif /* CONFIG_SYS_VSNPRINTF */

void udelay(uint32_t usec);
void mdelay(unsigned long msec);

void *memcpy(void *s1, const void *s2, size_t n);
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
int
print_buffer(unsigned long addr, const void* data, unsigned int width,
		  unsigned int count, unsigned int linelen);

/**
 * print sizes as "xxx KiB", "xxx.y KiB", "xxx MiB", "xxx.y MiB",
 * xxx GiB, xxx.y GiB, etc as needed; allow for optional trailing string
 * (like "\n")
 *
 * @param size	size to print
 * @param[out] s	string to append on the end (i.e. \n)
 */
void print_size(unsigned long long size, const char *s);

/**
 * Given a pointer to a virtual address return the physical address
 *
 * @param ptr	pointer to virtual, KSEG0 or XKPHYS address
 *
 * @return physical address of pointer
 */
uint64_t octeon_ptr_to_phys(void *ptr);

extern uint32_t __div64_32(uint64_t *n, uint32_t base);

/**
 * Resets the chip
 */
void octeon_do_reset(void);

/**
 * Returns the amount of time that has taken place since base in usec
 *
 * @param base	starting time
 *
 * @return number of microseconds that have elapsed since base time
 */
unsigned int get_timer(unsigned int base);

/**
 * Calculate the CRC32 checksum
 *
 * @param crc - starting crc
 * @param p - pointer to data to calculate CRC on
 * @param len - number of bytes to calculate CRC on
 *
 * @return crc32 of data
 */
uint32_t crc32(uint32_t crc, const void *p, size_t len);

/**
 * Searches for a valid bootloader following the ATE tool in the NOR flash.
 * If found it branches to the next bootloader.
 * If not found then 0 is returned.
 * NOTE: This resets the NOR flash start address on the boot bus to 0x1fc00000.
 */
int octeon_bootloader_scan(void);

/**
 * Sets the baudrate of the uart
 *
 * @param baudrate		Baud rate in bits per second
 * @param use_flow_control	!0 to enable hardware flow control
 */
void octeon_set_baudrate(int baudrate, int use_flow_control);

/**
 * Sets the current UART node and UART number
 * @param node		Output node to use
 * @param uart		Output UART to use
 */
void octeon_set_uart(int node, int uart);

/**
 * Returns the CPU speed in hz
 *
 * @return CPU speed in HZ
 */
uint64_t octeon_get_cpu_speed(void);

void *octeon_phys_to_ptr(uint64_t addr);

/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */
static inline int fls(int x)
{
	return (sizeof(x) * 8 - __builtin_clz(x));
}

extern void __octeon_init(void) __attribute__((noinline));

/**
 * Lock a single line into L2. The line is zeroed before locking
 * to make sure no dram accesses are made.
 *
 * @param addr   Physical address to lock
 */
void l2c_lock_line(uint64_t addr);

/**
 * Locks a memory region in the L2 cache
 *
 * @param start - start address to begin locking
 * @param len - length in bytes to lock
 */
void l2c_lock_mem_region(uint64_t start, uint64_t len);

/**
 * Unlock a single line in the L2 cache.
 *
 * @param addr	Physical address to unlock
 *
 * @return Zero on success
 */
void l2c_unlock_line(uint64_t addr);

/**
 * Unlock a memory region in the L2 cache
 *
 * @param start - start address to unlock
 * @param len - length to unlock in bytes
 */
void l2c_unlock_mem_region(uint64_t start, uint64_t len);

int main(void);

#include "octeon.h"
#include "cvmx-clock.h"
#include "cvmx-bootloader.h"

/**
 *  Given valid header returns image size (data + header); or 0
 *
 * @param header - pointer to bootloader header
 *
 * @returns size of the image in bytes or 0 if the header is invalid.
 */
uint32_t octeon_get_image_size(const bootloader_header_t * header);

/**
 * This function searches the NOR flash for a normal bootloader and returns a
 * pointer to the beginning of it if found or NULL if not found.
 *
 * @returns pointer to bootloader in NOR flash or NULL if not found.
 */
bootloader_header_t *octeon_find_and_validate_normal_bootloader(void);

/**
 * Get clock rate based on the clock type.
 *
 * @param node  - CPU node number
 * @param clock - Enumeration of the clock type.
 * @return      - return the clock rate.
 */
uint64_t octeon_clock_get_rate_node(int node, cvmx_clock_t clock);

#endif /* __TINYLIBC_H__ */
