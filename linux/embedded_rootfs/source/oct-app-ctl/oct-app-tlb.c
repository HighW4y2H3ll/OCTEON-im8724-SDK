/***********************license start************************************
 * Copyright (c) 2013 Cavium (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact support@cavium.com
 *
 ***********************license end**************************************/

/**
 * @file
 *
 * $Id: oct-app-tlb.c 129661 2015-12-13 22:52:37Z cchavva $
 *
 * DESCRIPTION
 *
 * This module provides an interface for creating >prototype> TLB entries
 * in a memory table, not in the actual physical TLB, and functions
 * to use the <prototype> TLBs to access the memory that would be
 * mapped by these TLBs.
 *
 * This module is needed when booting a standalone application
 * or for adding cores to a running standalone application,
 * when access to the physical TLBs of rhe intended core is impossible,
 * so these <prototype> TLBs are created in memory and are later
 * populated into the target processor core by the initialization
 * routine inside the bootloader.
 */

#define _GNU_SOURCE		/* Enable mmap64(2) et al. */
#define _XOPEN_SOURCE 500	/* Enable pread(2) */


#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>


#include "oct-app.h"

static int oct_tlb_memset(
		const octeon_tlb_entry_t_bl * tlb_array,
		app_vaddr_t dest_virt,
		uint8_t val, size_t len);

static tlb_flags_t oct_tlb_flags[ CABS_TYPE_MAX ];

/*
 * Function to initialize the <oct_tlb_flags> array
 */
void oct_tlb_flags_init(cvmx_app_boot_record_t * cabr )
{
	memset( &oct_tlb_flags, 0, sizeof(oct_tlb_flags));

	oct_tlb_flags[ CABS_TYPE_TEXT ].seg_type	= RO_TLB_SEGMENT;
	oct_tlb_flags[ CABS_TYPE_RO_DATA ].seg_type	= RO_TLB_SEGMENT;
	oct_tlb_flags[ CABS_TYPE_RW_SHARED ].seg_type	= SHARED_TLB_SEGMENT;
	oct_tlb_flags[ CABS_TYPE_RW_PRIVATE ].seg_type	= RW_TLB_SEGMENT;

	if( cabr->no_mark_private_data )
		oct_tlb_flags[ CABS_TYPE_RW_PRIVATE ].cca	= 0x7;

	/* Inhibuit execution on all but the .text segments */
	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX)) {
		oct_tlb_flags[ CABS_TYPE_TEXT ].xi		= 0;
		oct_tlb_flags[ CABS_TYPE_RO_DATA ].xi		= 1;
		oct_tlb_flags[ CABS_TYPE_RW_SHARED ].xi		= 1;
		oct_tlb_flags[ CABS_TYPE_RW_PRIVATE ].xi	= 1;
	}

	/* XXX no "write-inhibit" bit - how to mark text read-only ? XXX */

	/* Make special flags for stack+heap segment */
	oct_tlb_flags[ CABS_TYPE_STACK_HEAP ] =
		oct_tlb_flags[ CABS_TYPE_RW_PRIVATE ];
	oct_tlb_flags[ CABS_TYPE_STACK_HEAP ].seg_type = STACK_HEAP_TLB_SEGMENT;

}

void oct_tlb_stack_exec(bool stack_exec)
{
	oct_tlb_flags[ CABS_TYPE_STACK_HEAP ].xi = !stack_exec;
}

/*
 * Routines for accessing physical memory ranges
 * from Linux userland
 */

/*
 * Map a physical address range into userland virtual memory
 * Returns the actual mapped virtual range in <vrange> so that it can be
 * unmapped later.
 * Retrurn value is virtual address of corresponding to <phys_base>
 * as requested, or NULL on error.
 */
void * map_phys_range( paddr_t phys_base, size_t phys_size, 
		vmap_range_t * vrange )
{
	size_t pg_sz = sysconf(_SC_PAGESIZE), length;
	off64_t offset;
	void * vaddr;
	extern int devmem_fd;
	unsigned voff;

	if( vrange != NULL ) {
		vrange->vlength = 0;
	}

	/* Align /dev/mem offset to page */
	offset = phys_base & ~(pg_sz - 1);
	voff = phys_base - offset ;

	/* Calculate mapped length to be multiple of page size */
	length = phys_size + voff;
	length += pg_sz - 1;
	length &= ~(pg_sz - 1);

	vaddr = mmap64(NULL, length,
		PROT_READ|PROT_WRITE, MAP_SHARED,
		devmem_fd,
		offset );
	if (vaddr  == MAP_FAILED) {
		perror("mmap");
		return NULL;
	}

	/* Return actual virtual range mapped */
	if( vrange != NULL ) {
		vrange->vbase = vaddr;
		vrange->vlength = length;
	}

	return vaddr + voff;
}

/*
 * Release a mapping created by map_phys_range()
 */
int unmap_phys_range( vmap_range_t * vrange )
{
	int ret ;
	if( vrange != NULL ) {
		ret = munmap( vrange->vbase, vrange->vlength );

		vrange->vlength = 0;
		return ret ;
	} else
		return -1;

}

static int memcpy_virt_to_phys(
	paddr_t dest_addr, 
	const void * src_addr, 
	size_t count)
{
	vmap_range_t vrange;
	void * vaddr;

	vaddr = map_phys_range( dest_addr, count, &vrange );
	if( vaddr == NULL )
		return -1;

	memcpy(vaddr, src_addr, count);

	(void) unmap_phys_range( &vrange );
	return count;
}

static int memset_phys( paddr_t addr, uint8_t value, size_t len)
{
	vmap_range_t vrange;
	void *vaddr;

	vaddr = map_phys_range( addr, len, &vrange);
	if( vaddr == NULL )
		return -1;
	memset(vaddr, value, len );
	(void) unmap_phys_range( &vrange );
	return len;
}

static int memcpy_phys_to_phys( paddr_t dst_addr, paddr_t src_addr, size_t len)
{
	vmap_range_t vrd, vrs;
	void * vad;
	void * vas;
	int ret;

	vad = vas = NULL;

	vad = map_phys_range( dst_addr, len, &vrd );
	vas = map_phys_range( src_addr, len, &vrs );

	if( vad != NULL && vas != NULL ) {
		memcpy( vad, vas, len );
		ret = len;
	} else
		ret = -1;

	if( vad != NULL )
		unmap_phys_range( &vrd );

	if( vas != NULL )
		unmap_phys_range( &vrs );

	return ret ;
}



static const char * oct_tlb_entry_seg_type( unsigned ty )
{
	switch( ty ) {
		case RO_TLB_SEGMENT:
			return "RO";
		case RW_TLB_SEGMENT:
			return "RW";
		case SHARED_TLB_SEGMENT:
			return "SH";
		case STACK_HEAP_TLB_SEGMENT:
			return "SS";
		default:
			return "??";
	}
}

#if 0
void oct_tlb_entry_print(const octeon_tlb_entry_t_bl * te, const char * info)
{

	fprintf(stderr, "DEBUG: %s pg0=%llx pg1=%llx \n"
		"DEBUG: -- vbase=%llx pg_sz=%x\n ri=%x xi=%x cca=%x type=%s\n",
		info,
		(unsigned long long) te->phy_page0,
		(unsigned long long) te->phy_page1,
		(unsigned long long) te->virt_base,
		te->page_size,
		te->flags.ri,
		te->flags.xi,
		te->flags.cca,
		oct_tlb_entry_seg_type( te->flags.seg_type )
		);
}
#endif

void oct_tlb_array_print(const octeon_tlb_entry_t_bl * tlb_array,
	const char * info)
{
	int i, p;
	const int num_entries = NUM_OCTEON_TLB_ENTRIES;

	printf("TLB %s at vaddr: %p\n", info, tlb_array);
	for (i = 0; i < num_entries; i++) {
		if (tlb_array[i].page_size == 0)
			continue;
		printf("TLB[%02d]", i );
		printf(" page0: %#010llx", 
			(unsigned long long) tlb_array[i].phy_page0);
		if(tlb_array[i].phy_page1 != ~0ULL) {
			printf(" page1: %#010llx", 
				(unsigned long long) tlb_array[i].phy_page1);
			p = 1;
		} else {
			p = 0;
		}
		printf(" pagesize:%#08lx (%#x)",
		       TLB_PAGE_SIZE_FROM_EXP(tlb_array[i].page_size),
		       tlb_array[i].page_size);
		printf("\n   virt : %#llx - %#llx ",
		       tlb_array[i].virt_base + 0ULL,
		       tlb_array[i].virt_base +
		       (1ULL << (tlb_array[i].page_size + p)));
		printf( "ri=%x xi=%x cca=%x type=%s \n",
			tlb_array[i].flags.ri,
			tlb_array[i].flags.xi,
			tlb_array[i].flags.cca,
			oct_tlb_entry_seg_type( tlb_array[i].flags.seg_type ));
	}
}

static int oct_tlb_array_size(void)
{
    if (OCTEON_IS_MODEL(OCTEON_CN3XXX))
        return(32);
    if (OCTEON_IS_MODEL(OCTEON_CN5XXX))
        return(64);
    return NUM_OCTEON_TLB_ENTRIES;
}


/*
 * Compute page size needed to map a given application virtual
 * memory range.
 * Returns the log2 of page size.
 * Note that the virtual mapping consists of two pages, 
 * so the ectual virtual mapping spans 2^(n+1) bytes
 *
 * Note: only even page size exponents are supported in MIPS
 */
static unsigned int oct_tlb_size_mapping(
	app_vaddr_t	range_base,
	size_t		range_size
	)
{
	unsigned page_size_exp ;
	const size_t min_size_exp = 12;	/* Min page size 4KiB */
	const size_t max_size_exp = 24;	/* Max page size 16MiB */
	size_t	ext_range;

	/* Find the smallest mapping that will contain the entire range */
	for( page_size_exp = min_size_exp;
		page_size_exp < max_size_exp;
		page_size_exp +=2 ) {

		ext_range = range_size ;
		ext_range += range_base & ((1u << (page_size_exp+1))-1);

		if( ext_range <= (1u << (page_size_exp+1)))
			return page_size_exp;
	} /* for page_size_exp */

	return max_size_exp;
}

/*
 * Detect overlapping of mapping entries -
 * this function will iterate through the TLB entries and return
 * non-zero of any of these entries maps any portion of
 * the app virtual address space denoted by <v_base> .. <v_base>+<v_size>-1
 */
static int oct_tlb_range_is_mapped( 
		octeon_tlb_entry_t_bl * tlb_array,
		app_vaddr_t v_base, size_t v_size
		)
{
	unsigned tlb_index, tlb_num_entries;
	size_t vmap_size ;

	tlb_num_entries = oct_tlb_array_size() ;

	for( tlb_index = 0; tlb_index < tlb_num_entries; tlb_index ++) {
		/* Skip vacant entry */
		if( tlb_array[ tlb_index ].page_size == 0 )
			continue;

		/* mapped ragion higher than argument */
		if( tlb_array[ tlb_index ].virt_base > (v_base + v_size ))
			continue;

		vmap_size = 1ull << ( tlb_array[ tlb_index ].page_size + 1 );

		/* mapped region is lower than argument */
		if((tlb_array[ tlb_index ].virt_base + vmap_size - 1) < v_base )
			continue;

		fprintf(stderr, "ERROR: "
			"%s: range %#llx-%#llx overlaps TLB entry %u\n",
			__FUNCTION__,
			(unsigned long long) v_base,
			(unsigned long long) v_base + v_size - 1,
			tlb_index );
		return 1;
	}
	/* No overlap was found */
	return 0;
}


/*
 * Allocate a single physical page (a.k.a. frame) of specific size
 * where page_size = 2^page_size_exp
 * Returns physical address or 0 on error.
 */
static paddr_t oct_tlb_alloc_frame( unsigned page_size_exp )
{
	paddr_t page ;

	page =  cvmx_bootmem_phy_alloc(
			1ul << page_size_exp,	/* num bytes */
			0,		/* min phys addr */
			0,		/* max phys addr */
			1ull << page_size_exp,	/* alignment */
			 0);
	if( page == ~0ULL )
		return 0;
	return page;
}

/*
 * Add a single mapping entry to the TLB array
 * mapping at one pair of pages into a virtual range
 * Returns size of mapped range, could be less then requested <v_size>
 * or -1 on error.
 */
static int oct_tlb_map_single(
		octeon_tlb_entry_t_bl * tlb_array,
		app_vaddr_t v_base,
		size_t v_size,
		tlb_flags_t tlb_flags
		)
{
	unsigned page_size_exp;
	int mapped_size;
	size_t page_size;
	paddr_t page0, page1;
	unsigned tlb_index, tlb_num_entries;
	app_vaddr_t vmap_base, vmap_mask ;

	page0 = page1 = 0ull;
	page_size_exp = oct_tlb_size_mapping( v_base, v_size );

	/* Find first free TLB entry */
	tlb_num_entries = oct_tlb_array_size() ;

	assert( tlb_array != NULL);

	for( tlb_index = 0; tlb_index < tlb_num_entries; tlb_index ++)
		if( tlb_array[ tlb_index].page_size == 0)
			break;
	if(tlb_index >= tlb_num_entries ) {
		fprintf(stderr, "ERROR: out TLB entries\n");
		return -1;
	}

	if(verbose>2) fprintf(stderr, "DEBUG: "
		"Mapping vaddr %#llx-%#llx pg_sz=%u TLB[%02u]\n",
		(unsigned long long)v_base, 
		(unsigned long long)v_base + v_size -1,
		page_size_exp, tlb_index );

	/* Calculate base of virtual map */
	vmap_mask = (1ull <<  (page_size_exp+1)) -1 ;
	vmap_base = v_base & ~(vmap_mask);
	page_size = 1u << page_size_exp ;

	/* check for overlapping map */
	if( oct_tlb_range_is_mapped( tlb_array, v_base, v_size ) ){
		return -1;
	}

	/*
	 * NOTE:
	 * Bootloaser insists that page0 is always valid,
	 * so there are only two TLB types, with only page0,
	 * or with both page0 and page1 being valid
	 */
	if( (v_base + v_size) <= (vmap_base + page_size)) {
		/* Even page only */
		page0 = oct_tlb_alloc_frame( page_size_exp );
		page1 = ~0ULL;
	} else {
		/* Both pages needed */
		page0 = oct_tlb_alloc_frame( page_size_exp );
		page1 = oct_tlb_alloc_frame( page_size_exp );
	}

	if( page0 == 0ULL || page1 == 0ULL ) {
		fprintf(stderr, "ERROR: out of physical boot memory\n");
		return -1;
	}

	/* Calculate size actually mapped */
	if( (vmap_base + vmap_mask + 1) >= (v_base + v_size))
		mapped_size = v_size;
	else
		mapped_size = ((vmap_base + vmap_mask + 1) - v_base );


	if( page1 != ~0ull ) {
		if(verbose>3) fprintf(stderr, "DEBUG: Mapped page0=%#llx page1=%#llx"
				" size %#x/%#x bytes\n",
		(unsigned long long) page0, (unsigned long long) page1,
		mapped_size, 2 << page_size_exp );
	} else {
		if(verbose>3) fprintf(stderr, "DEBUG: Mapped page0=%#llx"
				" size %#x/%#x bytes\n",
		(unsigned long long) page0,
		mapped_size, 1 << page_size_exp );
	}

	/* Fill TLB entry with result */
	tlb_array[tlb_index].page_size = page_size_exp;
	tlb_array[tlb_index].phy_page0 = page0;
	tlb_array[tlb_index].phy_page1 = page1;
	tlb_array[tlb_index].virt_base = vmap_base;
	tlb_array[tlb_index].flags = tlb_flags;

	/* Return actual mapped size */
	return mapped_size;
}

/*
 * Create a mapping on the app target virtual space for
 * an entire region defined by target virtual address range,
 * and allocate physical memory underneath.
 * 
 * Returnns -1 on error, 0 on success.
 */
static int oct_tlb_map_create( octeon_tlb_entry_t_bl * tlb_array,
		app_vaddr_t v_base, size_t v_size,
		tlb_flags_t tlb_flags
		)
{
	int m_size;

	while( v_size > 0 ){
		m_size = oct_tlb_map_single( tlb_array, v_base, v_size,
				tlb_flags );

		/* returned value is the size that has been mapped */
		if( m_size <= 0 )
			return -1;

		v_size -= m_size;
		v_base += m_size;
	}
	return 0;
}

/*
 * Clone all shared sements to new TLB array
 */
static int oct_tlb_shared_clone( 
		octeon_tlb_entry_t_bl * dst_array,
		const octeon_tlb_entry_t_bl * src_array
		)
{
	unsigned i, j, tlb_num_entries;

	tlb_num_entries = oct_tlb_array_size() ;

	for (j = i = 0; i < tlb_num_entries; i++) {
		/* Unused entries are at the end */
		if ( 0 == src_array[i].page_size )
			break;

		/* Skip non-shared segments */
		if( src_array[i].flags.seg_type != RO_TLB_SEGMENT &&
		    src_array[i].flags.seg_type != SHARED_TLB_SEGMENT )
			continue;

		/* Find a vacant entry in the destination map */
		for( ; j < tlb_num_entries; j++ )
			if( dst_array[j].page_size == 0 )
				break;

		if( j >= tlb_num_entries ) {
			fprintf(stderr, "ERROR: "
			"Ran out of TLB entries for clonning shared segments\n"
			);
			return -1;
		}
		
		dst_array[j] = src_array[i];
	}
	return j ;
}


/*
 * Calculate the virtual base address of the application heap+stack segment
 * Initialize the heap & stack TLB entries and mark the pages that need
 * to be allocated wuth "0", and the page that is not needed with "-1".
 *
 * Returns the virtual address for heap+stack, NULL on error.
 */
app_vaddr_t oct_tlb_primary_heap_stack_locate(
		octeon_tlb_entry_t_bl * tlb_array,
		size_t heap_size,
		size_t stack_size
		)
{
	app_vaddr_t vaddr, vaddr_max;
	size_t page_size, map_size;
	tlb_flags_t tlb_flags;
	unsigned i, n;

	n = oct_tlb_array_size();
	vaddr_max = 0ull;

	/* Scan all segments for maximum virtual address used by app */
	for( i = 0; i < n; i ++ ) {
		if( tlb_array[i].page_size == 0 )
			break;

		vaddr = tlb_array[i].virt_base + 
			TLB_PAGE_SIZE_FROM_EXP( tlb_array[i].page_size+1);

		if( vaddr > vaddr_max )
			vaddr_max = vaddr ;

	}

	if( i >= n || vaddr_max == 0ull )
		return 0ull;

	/* Align the max vaddr to 32MiB */
	vaddr_max += (1 << 25) - 1;
	vaddr_max &= ~( (1 << 25) -1 );

	/* Set flags based on private data segment */
	tlb_flags =         oct_tlb_flags[ CABS_TYPE_RW_PRIVATE ];
	tlb_flags.seg_type  = STACK_HEAP_TLB_SEGMENT ;

	page_size = oct_tlb_size_mapping( vaddr_max, heap_size + stack_size );

	tlb_array[i].virt_base = vaddr_max;
	tlb_array[i].page_size = page_size;
	tlb_array[i].flags = tlb_flags;

	/* Create empty mapping for the heap and stack */
	tlb_array[i].phy_page0 = 0ULL;
	tlb_array[i].phy_page1 = 0ULL;

	/* Mark with "-1" if the segment only needs one page of two */
	map_size = TLB_PAGE_SIZE_FROM_EXP(page_size);
	if( map_size<= (heap_size+stack_size)) {
		tlb_array[i].phy_page1 = ~0ULL;
	} else {
		map_size <<= 1;
	}

	if(verbose) fprintf(stderr, "INFO: "
		" heap at vaddr %#llx size %#x/%#x seg %d\n",
		(unsigned long long) vaddr_max,
		(unsigned) (heap_size+stack_size),
		(unsigned) map_size, i );

	return vaddr_max;
}


/*
 * Clone all private sements to a new TLB array
 * by allocating new pages for the new mapping
 * and copying the content of these pages from
 * the primary map.
 * Also, allocate the pages for the heap+stack segment.
 */
static int oct_tlb_private_clone( 
		octeon_tlb_entry_t_bl * dst_array,
		const octeon_tlb_entry_t_bl * src_array
		)
{
	unsigned i, j, pge, tlb_num_entries;
	int ret = 0;

	tlb_num_entries = oct_tlb_array_size() ;

	for (i = j = 0; i < tlb_num_entries; i++) {
		/* Unused entries are at the end */
		if ( 0 == (pge = src_array[i].page_size) )
			break;

		if( src_array[i].flags.seg_type != RW_TLB_SEGMENT &&
		    src_array[i].flags.seg_type != STACK_HEAP_TLB_SEGMENT ) 
			continue;

		/* find unused entry in destination map */
		for( ; j < tlb_num_entries; j ++ )
			if( dst_array[j].page_size == 0 )
				break;

		/* did we run out of destination map entries ? */
		if( j >= tlb_num_entries ) {
			fprintf(stderr, "ERROR: "
				" clone private: out of TLB map entries\n" );
			ret = -1;
			break;
		}

		/* Copy the invariant fields */
		dst_array[j].page_size = pge;
		dst_array[j].virt_base = src_array[i].virt_base ;
		dst_array[j].flags = src_array[i].flags ;
		/* Preset pages to nils */
		dst_array[j].phy_page0 = dst_array[j].phy_page1 = ~0ULL;

		/* Allocate pages for new mapping and copy theur contents */
		if( src_array[i].phy_page0 != ~0ULL ) {
			dst_array[j].phy_page0 = oct_tlb_alloc_frame( pge );
			if( dst_array[j].phy_page0 == 0ull )
				ret = -1;
			else if( src_array[i].phy_page0 != 0ull )
				ret = memcpy_phys_to_phys(
						dst_array[j].phy_page0,
						src_array[i].phy_page0,
						1u << pge );
			else
				ret = memset_phys(
						dst_array[j].phy_page0,
						0xEE,
						1u << pge );
		}
		if( ret < 0 ) {
			fprintf(stderr, "ERROR: "
				" cloning private: failed to copy page0\n");
			break;
		}

		if( src_array[i].phy_page1 != ~0ULL ) {
			dst_array[j].phy_page1 = oct_tlb_alloc_frame( pge );
			if( dst_array[j].phy_page1 == 0ull )
				ret = -1;
			else if( src_array[i].phy_page0 != 0ull )
				ret = memcpy_phys_to_phys(
						dst_array[j].phy_page1,
						src_array[i].phy_page1,
						1u << pge );
			else
				ret = memset_phys(
						dst_array[j].phy_page0,
						0xEE,
						1u << pge );
		}
		if( ret < 0 ) {
			fprintf(stderr, "ERROR: "
				" cloning private: failed to copy page1\n");
			break;
		}

	} /* for i = 0 .. tlb_entries */

	if( ret < 0 )
		fprintf(stderr, "ERROR: "
			" failed to clone private segment\n");

	return ret ;
}

/*
 * Fill an application virtual address range with a constant value.
 * Useful for initialzing the BSS section of the private data segment
 *
 * Returns length on success, or a negative residual length on error
 */
static int oct_tlb_memset(
		const octeon_tlb_entry_t_bl * tlb_array,
		app_vaddr_t dest_virt,
		uint8_t val, size_t len)
{
	paddr_t  dest_phys;
	app_vaddr_t vaddr;
	size_t cur_len;
	size_t chunk_len;
	size_t page_size;
	unsigned int off;
	unsigned tix, p, i, n;
	int ret = 0;

	if(verbose>1) fprintf(stderr, "INFO: "
		"Clearing vaddr=%#llx len=%#x\n",
		(unsigned long long) dest_virt, (unsigned) len );

	assert(len < 1<<25);

	n = oct_tlb_array_size() << 1;

	cur_len = len;

	for (tix = 0; tix < n && cur_len > 0; tix++) {

		i = tix >> 1;	/* 0..127 TLB index */
		p = tix & 1;	/* 0..1 page index within TLB */

		/* empty TLB acts as terminator */
		if( tlb_array[i].page_size == 0 )
			break;

		page_size = TLB_PAGE_SIZE_FROM_EXP(tlb_array[i].page_size);

		/* app virtual address for this page */
		vaddr =  tlb_array[i].virt_base;
		if( p != 0 )
			vaddr += page_size;

		/* If target address falls within range */
		if (dest_virt >= vaddr &&
		    dest_virt < (vaddr + page_size)) {

			if( p == 0 )
				dest_phys = tlb_array[i].phy_page0 ;
			else
				dest_phys = tlb_array[i].phy_page1 ;

			off = dest_virt - vaddr;
			dest_phys += off;

			chunk_len = MIN(page_size - off, cur_len);

			if(verbose>1) fprintf(stderr, "INFO: "
				"Clearing %#x bytes at Phys%#llx Virt%#llx\n",
			        (unsigned) chunk_len,
				(unsigned long long) dest_phys,
				(unsigned long long) dest_virt);

			/* do the deed */
			ret = memset_phys(dest_phys, val, chunk_len);
			if( ret < 0 )
				break;

			cur_len -= chunk_len;
			dest_virt += chunk_len;
		}
	} /* for tix */

	if (cur_len != 0) {
		fprintf(stderr, "ERROR: "
			" clearing mapped memory, done %#x of %#x\n",
				(unsigned) cur_len, (unsigned) len);

		return (cur_len - len);
	} else {
		return (len);
	}
}

/*
 * Fill an application virtual address range with data from buffer
 * Useful for initialzing the Code/Data segments from object-file
 *
 * Returns length on success, or a negative residual length on error
 */
int oct_tlb_memcpy(
		const octeon_tlb_entry_t_bl * tlb_array,
		app_vaddr_t dest_virt,
		const void * buf, size_t len)
{
	paddr_t  dest_phys;
	app_vaddr_t vaddr;
	size_t cur_len;
	size_t chunk_len;
	size_t page_size;
	unsigned int off;
	unsigned tix, p, i, n;
	int ret = 0;

	n = oct_tlb_array_size() << 1;

	assert(len < 1<<25);

	if(verbose>1) fprintf(stderr, "INFO: "
		"Copying to vaddr=%#llx len=%#x\n",
		(unsigned long long) dest_virt, (unsigned) len );

	cur_len = len;

	for (tix = 0; tix < n && cur_len > 0; tix++) {

		i = tix >> 1;	/* 0..127 TLB index */
		p = tix & 1;	/* 0..1 page index within TLB */

		/* empty TLB acts as terminator */
		if( tlb_array[i].page_size == 0 )
			break;

		page_size = TLB_PAGE_SIZE_FROM_EXP(tlb_array[i].page_size);

		/* app virtual address for this page */
		vaddr =  tlb_array[i].virt_base;
		dest_phys = tlb_array[i].phy_page0 ;

		/* Correct for odd pages */
		if( p != 0 ) {
			vaddr += page_size;
			dest_phys = tlb_array[i].phy_page1 ;
		}

		/* If target address falls within range */
		if (dest_virt >= vaddr &&
		    dest_virt < (vaddr + page_size)) {

			off = dest_virt - vaddr;
			dest_phys += off;

			chunk_len = MIN(page_size - off, cur_len);

			if(verbose>2) fprintf(stderr, "INFO: "
				"Copying %#x bytes at Phys%#llx Virt%#llx\n",
			        (unsigned) chunk_len,
				(unsigned long long) dest_phys,
				(unsigned long long) dest_virt);

			/* do the deed */
			ret = memcpy_virt_to_phys(dest_phys, buf, chunk_len);
			if( ret < 0 )
				break;

			cur_len -= chunk_len;
			dest_virt += chunk_len;
		}
	} /* for tix */

	if (cur_len != 0) {
		fprintf(stderr, "ERROR: "
			" copying to mapped memory, remains %#x of %#x\n",
				(unsigned) cur_len, (unsigned) len);

		return (cur_len - len);
	} else {
		return (len);
	}
}


/*
 * Initialize a primary TLB map from ELF onject-file
 *
 * In the primary map, the private r/w data segment(s) are loaded
 * with the object-file initializer segment content, of one exists,
 * or zero-out the content if there are no initializers in the file.
 * This function needs to be called only once per application.
 *
 * NOTE: the primary table actually allocates physical memory
 * to contain the BSS sections, which seems wasteful,
 * but elegant at the same time.
 */
int oct_tlb_primary_init(
		int elf_fd,
		octeon_tlb_entry_t_bl * tlb_array,
		cvmx_app_boot_segment_t * seg,
		unsigned num_seg
		)
{
	unsigned six;	/* segment index */
	void * buf;
	const size_t bufsiz = 1 << 16;
	app_vaddr_t vaddr ;
	size_t len;
	off_t off;
	int rc, ret ;

	assert( seg != NULL );
	assert( tlb_array != NULL );

	/* Allocate temporary data buffer */
	buf = malloc( bufsiz );
	if( buf == NULL)
		return -1;

	for( ret = six = 0; six < num_seg; six ++ ) {
		tlb_flags_t tlb_flags;

		/* Empty segment is at the end */
		if( seg[six].cabs_type == CABS_TYPE_NONE )
			break;

		if(verbose>2) fprintf(stderr, "DEBUG: "
			"primary-map seg %u type %d"
			" addr %#llx size %#llx\n",
			 six, seg[six].cabs_type ,
			(unsigned long long) seg[six].vaddr,
			(unsigned long long) seg[six].mem_size );

		/* Set TLB flags according to segment type */
		switch( seg[six].cabs_type ){
			case CABS_TYPE_TEXT:
			case CABS_TYPE_RO_DATA:
			case CABS_TYPE_RW_SHARED:
			case CABS_TYPE_RW_PRIVATE:
				tlb_flags = oct_tlb_flags[ seg[six].cabs_type ];
				break;
			default:
				ret = -1;
				break;
		}
		if( ret < 0 )
			break;

		/* Create the virtual memoryu range */
		ret =  oct_tlb_map_create( tlb_array, 
			seg[six].vaddr,
			seg[six].mem_size,
			tlb_flags );
		if( ret < 0 )
			break;

		/* Copy content of the entire segment from object-file */
		rc = 0;
		vaddr = seg[six].vaddr;
		len = seg[six].file_size,
		off = seg[six].file_offset;

		while( len > 0 ) {
			unsigned rs;
			rs = MIN(bufsiz, len);
			rc = pread( elf_fd, buf, rs, off );
			if( rc <= 0 ) {
				fprintf(stderr, "ERROR: "
					"failed to read object-file\n");
				break;
			}

			ret = oct_tlb_memcpy( tlb_array, vaddr, buf, rc );
			if( ret <= 0 ) {
				rc = -1;
				break;
			}
			vaddr += ret ;
			off += ret ;
			len -= ret;
		}

		if( rc < 0 ) {
			ret = -1;
			break;
		}

		if( ret < 0 )
			break;


		/* Clear the remainder if file size is less then mem size */
		len = seg[six].mem_size - seg[six].file_size ;
		if( len > 0 ) {
			vaddr = seg[six].vaddr + seg[six].file_size;
			ret = oct_tlb_memset( tlb_array, vaddr, 0, len );
		}
		if( ret < 0 )
			break;
	} /* for six */

	free( buf );

	/* Print target TLB mapping */
	if( verbose > 1 )
		oct_tlb_array_print( tlb_array, "Primary map" );

	return ret;
}

/*
 * Initialize a secondary core TLB map from a primary TLB map
 *
 * The primary TLB array contains all required segments, including
 * the intialized private data segment, but is never used by any
 * running core, therefore its initial data section content remain
 * unchanged and can be used to bootstrap the mapped memory
 * for any core to use, including the initial core for an app.
 * This function should be called once for every core running an app.
 */
int oct_app_memory_clone(
		const octeon_tlb_entry_t_bl * primary_array,
		octeon_tlb_entry_t_bl * new_array
		)
{
	int ret ;

	/* Create Text, shared mapping */
	ret =  oct_tlb_shared_clone(  new_array, primary_array );
	if( ret < 0 )
		return ret;

	/* Create private data and heap/strack */
	ret =  oct_tlb_private_clone(  new_array, primary_array );
	if( ret < 0 )
		return ret ;

	/* For debug */
	if(verbose>1)
		oct_tlb_array_print( new_array, "Per-core map" );

	return ret;
}

/*
 * Release memory mapped by a TLB array
 *
 * When <primary> is true, all pages are released, otherwise only
 * pagtes belonging to private segments are deallocated.
 */
int oct_app_memory_free(
		const octeon_tlb_entry_t_bl * tlb_array,
		bool primary
		)
{
	unsigned tlb_num_entries;
	unsigned i;
	unsigned pge;
	bool shared;
	paddr_t page0, page1;
	int ret0, ret1;

	tlb_num_entries = oct_tlb_array_size() ;

	/* Print target TLB mapping */
	if( verbose > 1 ) {
		fprintf(stderr, "INFO: "
			"Freeling mapped %s memory\n",
			(primary)? "primary" : "pre-core");
		oct_tlb_array_print( tlb_array, "Freeing map" );
	}

	for (i = 0; i < tlb_num_entries; i++) {
		/* Unused entries are at the end */
		if ( 0 == (pge = tlb_array[i].page_size) )
			break;

		if( tlb_array[i].flags.seg_type != RW_TLB_SEGMENT &&
		    tlb_array[i].flags.seg_type != STACK_HEAP_TLB_SEGMENT ) 
			shared = 1;
		else
			shared = 0;

		/* Skip shared segments unless primary switch is on */
		if( !primary && shared )
			continue;

		page0 = tlb_array[i].phy_page0;
		page1 = tlb_array[i].phy_page1;

		/* _phy_free() returns 1 on success, 0 on error */
		ret0 = ret1 = 1;

		/* Old convention: dup page# if half of VM range is needed */
		if( page1 == page0 ) {
			page1 = ~0ull;
		}

		if( page0 != 0ull && page0 != ~0ull )
			ret0 = __cvmx_bootmem_phy_free( 
				page0,			/* phys addr */
				1ul << pge,		/* num bytes */
				0 );			/* no flags */

		if( ret0 <= 0 )
			fprintf(stderr, "WARNING: "
				"TLB[%d]: Error freeing physical page0 %#llx\n",
				i, (unsigned long long) page0 );

		if( page1 != 0ull && page1 != ~0ull )
			ret1 = __cvmx_bootmem_phy_free( 
				page1,			/* phys addr */
				1ul << pge,		/* num bytes */
				0 );			/* no flags */

		if( ret1 <= 0 )
			fprintf(stderr, "WARNING: "
				"TLB[%d]: Error freeing physical page1 %#llx\n",
				i, (unsigned long long) page1 );

	}
	return 0;
}


/* End of File */
