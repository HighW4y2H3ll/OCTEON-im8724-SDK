/* Note that this was taken from newlib.  Octeon is not size constrained */

/*
FUNCTION
        <<memcpy>>---copy memory regions

ANSI_SYNOPSIS
        #include <string.h>
        void* memcpy(void *<[out]>, const void *<[in]>, size_t <[n]>);

TRAD_SYNOPSIS
        void *memcpy(<[out]>, <[in]>, <[n]>
        void *<[out]>;
        void *<[in]>;
        size_t <[n]>;

DESCRIPTION
        This function copies <[n]> bytes from the memory region
        pointed to by <[in]> to the memory region pointed to by
        <[out]>.

        If the regions overlap, the behavior is undefined.

RETURNS
        <<memcpy>> returns a pointer to the first byte of the <[out]>
        region.

PORTABILITY
<<memcpy>> is ANSI C.

<<memcpy>> requires no supporting OS subroutines.

QUICKREF
        memcpy ansi pure
	*/

#include <linux/string.h>
#include <asm/arch/cvmx-asm.h>

/* Nonzero if either X or Y is not aligned on a "long" boundary.  */
#define UNALIGNED(X, Y) 				\
  (((long)X & (sizeof(long long) - 1)) |		\
   ((long)Y & (sizeof(long long) - 1)))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof(long long) << 2)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof(long long))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

void *memcpy(void *dst0, const void *src0, __kernel_size_t len0)
{
	char *dst = dst0;
	const char *src = src0;
	long long *aligned_dst;
	const long long *aligned_src;
	int len = len0;
	int i;

	/* If the size is small, or either SRC or DST is unaligned,
	   then punt into the byte copy loop.  This should be rare.  */
	CVMX_PREFETCH_PREF0(src, 0);

	if (!TOO_SMALL(len) && !UNALIGNED(src, dst)) {
		aligned_dst = (long long *)dst;
		aligned_src = (long long *)src;

		while (len > 128) {
			CVMX_PREFETCH_PREF0(aligned_src, 128);
			if (len > 256)
				CVMX_PREFETCH_PREF0(aligned_src, 256);
			for (i = 0; i < (128 / BIGBLOCKSIZE); i++) {
				*aligned_dst++ = *aligned_src++;
				*aligned_dst++ = *aligned_src++;
				*aligned_dst++ = *aligned_src++;
				*aligned_dst++ = *aligned_src++;
			}
			len -= 128;
		}

		/* Copy 4X long words at a time if possible.  */
		while (len >= BIGBLOCKSIZE) {
			*aligned_dst++ = *aligned_src++;
			*aligned_dst++ = *aligned_src++;
			*aligned_dst++ = *aligned_src++;
			*aligned_dst++ = *aligned_src++;
			len -= BIGBLOCKSIZE;
		}

		/* Copy one long word at a time if possible.  */
		while (len >= LITTLEBLOCKSIZE) {
			*aligned_dst++ = *aligned_src++;
			len -= LITTLEBLOCKSIZE;
		}

		/* Pick up any residual with a byte copier.  */
		dst = (char *)aligned_dst;
		src = (char *)aligned_src;
	}

	while (len--)
		*dst++ = *src++;

	return dst0;
}
