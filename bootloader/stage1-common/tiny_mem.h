#ifndef __TINY_MEM_H__
#define __TINY_MEM_H__

/**
 * Allocate more core for the heap
 *
 * @param	nu	number of units to allocate
 *
 * @return	pointer to new memory block or NULL if out of memory
 */
struct header *morecore(unsigned nu);

/**
 * Allocate memory from the heap
 *
 * @param	nbytes	Number of bytes to allocate
 *
 * @return	Pointer to allocated memory or NULL if out of memory
 */
void *malloc(size_t nbytes);

/**
 * Allocate space for nelem elements of size elsize and zero out the memory
 *
 * @param	nelem	Number of elements to allocate
 * @param	elsize	Size of each element
 *
 * @return	Pointer to zeroed block of memory or NULL if out of memory
 */
void *calloc(size_t nelem, size_t elsize);

/**
 * Returns memory back to the heap
 *
 * @param	ap	Pointer to memory allocated with malloc or calloc to free
 */
void free(void *ap);

#endif /* __TINY_MEM_H__ */
