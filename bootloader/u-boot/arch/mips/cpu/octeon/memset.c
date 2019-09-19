/* Note that this was taken from newlib */

/*
 * FUNCTION
 *	<<memset>>---set an area of memory
 *
 * INDEX
 *	memset
 *
 * ANSI_SYNOPSIS
 *	#include <string.h>
 *	void *memset(const void *<[dst]>, int <[c]>, size_t <[length]>);
 *
 * TRAD_SYNOPSIS
 *	#include <string.h>
 *	void *memset(<[dst]>, <[c]>, <[length]>)
 *	void *<[dst]>;
 *	int <[c]>;
 *	size_t <[length]>;
 *
 * DESCRIPTION
 *	This function converts the argument <[c]> into an unsigned
 *	char and fills the first <[length]> characters of the array
 *	pointed to by <[dst]> to the value.
 *
 * RETURNS
 *	<<memset>> returns the value of <[m]>.
 *
 * PORTABILITY
 * <<memset>> is ANSI C.
 *
 *    <<memset>> requires no supporting OS subroutines.
 *
 * QUICKREF
 *	memset ansi pure
 */

#include <linux/string.h>

#define LBLOCKSIZE (sizeof(long long))
#define UNALIGNED(X)   ((long)X & (LBLOCKSIZE - 1))
#define TOO_SMALL(LEN) ((LEN) < LBLOCKSIZE)

void *memset(void *m, int c, size_t n)
{
	char *s = (char *) m;
	int i;
	unsigned long long buffer;
	unsigned long long *aligned_addr;
	unsigned int d = c & 0xff;	/* To avoid sign extension,
					 * copy C to an unsigned variable.
					 */

	if (!TOO_SMALL (n) && !UNALIGNED (m)) {
		/* If we get this far, we know that n is large and m is word-aligned. */
		aligned_addr = (unsigned long long*)m;

		/* Store D into each char sized location in BUFFER so that
		 *         we can set large blocks quickly.  */
		if (LBLOCKSIZE == 4) {
			buffer = (d << 8) | d;
			buffer |= (buffer << 16);
		} else if (LBLOCKSIZE == 8) {
			buffer = (d << 8) | d;
			buffer |= (buffer << 16);
			buffer |= (buffer << 32);
		} else {
			buffer = d;
			for (i = 1; i < LBLOCKSIZE; i++)
				buffer = (buffer << 8) | d;
		}

		while (n >= LBLOCKSIZE*4) {
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			n -= 4*LBLOCKSIZE;
		}

		while (n >= LBLOCKSIZE) {
			*aligned_addr++ = buffer;
			n -= LBLOCKSIZE;
		}
		/* Pick up the remainder with a bytewise loop.  */
		s = (char*)aligned_addr;
	}

	while (n--) {
		*s++ = (char)d;
	}

	return m;
}
