/*
 *  linux/lib/string.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2014 Cavium, inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/arch/cvmx-ciu-defs.h>
#include <asm/arch/octeon-model.h>

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void *memset(void * s, int c, size_t count)
{
	unsigned long *sl = (unsigned long *) s;
	unsigned long cl = 0;
	char *s8;
	int i;

	/* do it one word at a time (32 bits or 64 bits) while possible */
	if ( ((ulong)s & (sizeof(*sl) - 1)) == 0) {
		for (i = 0; i < sizeof(*sl); i++) {
			cl <<= 8;
			cl |= c & 0xff;
		}
		while (count >= sizeof(*sl)) {
			*sl++ = cl;
			count -= sizeof(*sl);
		}
	}
	/* fill 8 bits at a time */
	s8 = (char *)sl;
	while (count--)
		*s8++ = c;

	return s;
}
void * memset(void * s, int c, size_t count) __attribute__((weak));

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void * memcpy(void *dest, const void *src, size_t count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* while all data is aligned (common case), copy a word at a time */
	if ( (((ulong)dest | (ulong)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}
void * memcpy(void *dest, const void *src, size_t count) __attribute__((weak));

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void * cs, const void * ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
		return res;
}
int memcmp(const void * cs, const void * ct, size_t count) __attribute__((weak));

/* Implement CRC using Octeon's CRC hardware unit.  This provides a significant
 * speed up, especially when the code is executing from flash (and the data
 * being CRC'ed is in flash, as is done during boot.)
 */
uint32_t crc32_no_comp(uint32_t crc, const uint8_t *buf, unsigned int len)
{
	volatile uint64_t *ptr64;
	volatile uint8_t *ptr = (void *)buf;
	uint32_t poly = 0;

	poly = 0x04c11db7; /* Bit reversed standard CRC32 polynomial - 0xedb88320 */
	CVMX_MT_CRC_POLYNOMIAL(poly);
	/* Do endian byte swap on CRC, then use reflect mode of
	 * MT IV to reverse the bits within the bytes.  This results
	 * in a bit-reversed IV. */
	CVMX_ES32(crc, crc);
	CVMX_MT_CRC_IV_REFLECT(crc);
	while ((((uint32_t)ptr) & 0x7) && len) {
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
uint32_t crc32_no_comp(uint32_t crc, const uint8_t *buf, unsigned int len)
__attribute__((weak));

uint32_t crc32 (uint32_t crc, const uint8_t *p, unsigned int len)
{
	return crc32_no_comp(crc ^ 0xffffffffL, p, len) ^ 0xffffffffL;
}
uint32_t crc32 (uint32_t crc, const uint8_t *p, unsigned int len)
__attribute__((weak));

/**
 * Pets the watchdog
 */
void hw_watchdog_reset(void)
{
	/* Pet the dog.  Good dog! */
	cvmx_write_csr(CVMX_CIU_PP_POKEX(0), 1);
}

void hw_watchdog_reset(void) __attribute__((weak));

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On MIPS it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}

int __octeon_is_model_runtime__(uint32_t model)
	__attribute__((weak));

int __octeon_is_model_runtime__(uint32_t model)
{
	return __octeon_is_model_runtime_internal__(model);
}
