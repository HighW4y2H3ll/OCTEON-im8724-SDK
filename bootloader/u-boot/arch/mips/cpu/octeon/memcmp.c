#include <common.h>
#include <asm/string.h>
#include <asm/arch/cvmx-asm.h>

/** Nonzero if either X or Y is not aligned on a 64-bit boundary */
#define UNALIGNED(X, Y) \
	(((uint64_t)((uintptr_t)(X)) & (sizeof(uint64_t) - 1)) \
	| ((uint64_t)((uintptr_t)(Y)) & (sizeof(uint64_t) - 1)))

/** How many bytes are copied each iteration of the word copy loop. */
#define LBLOCKSIZE	(sizeof(uint64_t))

/** Threshhold for punting to the byte copier. */
#define TOO_SMALL(LEN)	((LEN) < LBLOCKSIZE)

int memcmp(const void *m1, const void *m2, __kernel_size_t n)
{
	const u8 *s1 = (u8 *)m1;
	const u8 *s2 = (u8 *)m2;
	const u64 *a1, *a2;

	/* If the size is too small or either pointer is unaligned then we punt
	 * to the byte compare loop.  Hopefully this will not turn up in inner
	 * loops.
	 */
	CVMX_PREFETCH0(m1);
	CVMX_PREFETCH0(m2);
	if (!TOO_SMALL(n) && !UNALIGNED(s1, s2)) {
		/* Otherwise, load and compare the blocks of memory to use
		 * one word at a time.
		 */
		a1 = (u64 *)s1;
		a2 = (u64 *)s2;
		while (n >= LBLOCKSIZE) {
			if (*a1 != *a2)
				break;
			a1++;
			a2++;
			n -= LBLOCKSIZE;
		}
		/* check m mod LBLOCKSIZE remaining characters */
		s1 = (u8 *)a1;
		s2 = (u8 *)a2;
	} else if (!TOO_SMALL(n) && UNALIGNED(s1, s2)) {
		u64 t1, t2;
		while (n >= LBLOCKSIZE) {
			CVMX_LOADUNA_INT64(t1, s1, 0);
			CVMX_LOADUNA_INT64(t2, s2, 0);
			if (t1 != t2)
				break;
			s1 += LBLOCKSIZE;
			s2 += LBLOCKSIZE;
			n-= LBLOCKSIZE;
		}
	}
	/* Check m mod LBLOCKSIZE remaining characters */
	while (n--) {
		if (*s1 != *s2)
			return *s1 - *s2;
		s1++;
		s2++;
	}

	return 0;
}
