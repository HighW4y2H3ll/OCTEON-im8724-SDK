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

#include "tinylibc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <malloc.h>

#define NALLOC	1024	/* Minimum #units to request */

#ifndef CONFIG_HEAP_SIZE
# define CONFIG_HEAP_SIZE	0
#endif

extern unsigned long __end;

struct header {
	struct header 	*ptr;
	unsigned	size;
};

#if (CONFIG_HEAP_SIZE > 0)
static struct header base = { .ptr = NULL, .size = 0 };
static struct header *freep = NULL;
static struct header *sbrk_ptr = NULL;
#endif

/**
 * Allocate more core for the heap
 *
 * @param	nu	number of units to allocate
 *
 * @return	pointer to new memory block or NULL if out of memory
 */
struct header *morecore(unsigned nu __attribute__((unused)))
{
#if (CONFIG_HEAP_SIZE == 0)
	return NULL;
#else
	const char *cp;
	struct header *up;
	const char *eptr = (char *)&__end;
	unsigned num_bytes = nu * sizeof(struct header);

	if (nu < NALLOC)
		nu = NALLOC;
	if (sbrk_ptr == NULL) {
		/* __end is guaranteed to be 16-byte aligned by the linker */
		sbrk_ptr = (struct header *)eptr;
		sbrk_ptr++;
	}
	cp = (char *)sbrk_ptr;
 	if ((&cp[num_bytes]) > &eptr[CONFIG_HEAP_SIZE])
		return NULL;

	up = (struct header *)cp;
	up->size = nu;
	free((void *)(up + 1));
	return freep;
#endif
}

/**
 * Allocate memory from the heap
 *
 * @param	nbytes	Number of bytes to allocate
 *
 * @return	Pointer to allocated memory or NULL if out of memory
 */
void *malloc(size_t nbytes __attribute__((unused)))
{
#if (CONFIG_HEAP_SIZE == 0)
	return NULL;
#else
	struct header *p, *prevp;
	unsigned nunits;

	nunits = (nbytes + sizeof(struct header) - 1) / sizeof(struct header) + 1;
	if ((prevp = freep) == NULL) {
		base.ptr = freep = prevp = &base;
		base.size = 0;
	}

	for (p = prevp->ptr; ; prevp = p, p = p->ptr) {
		if (p->size >= nunits) {
			if (p->size == nunits) {
				prevp->ptr = p->ptr;
			} else {
				p->size -= nunits;
				p += p->size;
				p->size = nunits;
			}
			freep = prevp;
			return (void *)(p + 1);
		}
		if (p == freep) {
			if ((p = morecore(nunits)) == NULL)
				return NULL;
		}
	}
#endif
}

/**
 * Allocate space for nelem elements of size elsize and zero out the memory
 *
 * @param	nelem	Number of elements to allocate
 * @param	elsize	Size of each element
 *
 * @return	Pointer to zeroed block of memory or NULL if out of memory
 */
void *calloc(size_t nelem __attribute__((unused)),
	     size_t elsize __attribute__((unused)))
{
#if (CONFIG_HEAP_SIZE == 0)
	return NULL;
#else
	void *ptr = malloc(nelem * elsize);
	if (ptr)
		memset(ptr, 0, nelem * elsize);

	return ptr;
#endif
}

/**
 * Returns memory back to the heap
 *
 * @param	ap	Pointer to memory allocated with malloc or calloc to free
 */
void free(void *ap __attribute__((unused)))
{
#if (CONFIG_HEAP_SIZE > 0)
	struct header *bp, *p;

	bp = (struct header *)ap - 1;
	for (p = freep; !(bp > p && bp < p->ptr); p = p->ptr)
		if (p >= p->ptr && (bp > p || bp < p->ptr))
			break;	/* freed block at start or end of arena */
	if (bp + bp->size == p->ptr) { /* join to upper nbr */
		bp->size += p->ptr->size;
		bp->ptr = p->ptr->ptr;
	} else {
		bp->ptr = p->ptr;
	}
	if (p + p->size == bp) {
		p->size += bp->size;
		p->ptr = bp->ptr;
	} else {
		p->ptr = bp;
	}
	freep = p;
#endif
}
