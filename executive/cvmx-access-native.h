/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 * Functions for accessing memory and CSRs on Octeon when we are compiling
 * natively.
 *
 * <hr>$Revision: 38306 $<hr>
*/
#ifndef __CVMX_ACCESS_NATIVE_H__
#define __CVMX_ACCESS_NATIVE_H__
#include "cvmx-asm.h"
#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

#ifndef CVMX_DEFAULT_ROOT_NODE
#define CVMX_DEFAULT_ROOT_NODE 0
#endif

/**
 * Returns the Octeon processor ID.
 *
 * @return Octeon processor ID from COP0
 */
static inline uint32_t cvmx_get_proc_id(void)
{
#ifdef CVMX_BUILD_FOR_LINUX_USER
	extern CVMX_SHARED uint32_t cvmx_app_init_processor_id;
	return cvmx_app_init_processor_id;
#else
	uint32_t id;
	asm("mfc0 %0, $15,0":"=r"(id));
	return id;
#endif
}

/**
 * Convert a memory pointer (void*) into a hardware compatable
 * memory address (uint64_t). Octeon hardware widgets don't
 * understand logical addresses.
 *
 * @param ptr    C style memory pointer
 * @return Hardware physical address
 *
 * NOTE: In simple executive this function will not return correct data in many
 * cases and should only be used for memory allocated using cvmx_bootmem_alloc.
 * Using this function on global variables or on memory allocated from the
 * stack will return incorrect addresses.
 *
 * NOTE: Internal use only
 */
static inline __attribute__ ((__always_inline__))
uint64_t __cvmx_ptr_to_phys_internal__(void *ptr)
{
	if (CVMX_ENABLE_PARAMETER_CHECKING)
		cvmx_warn_if(ptr == NULL, "cvmx_ptr_to_phys() passed a NULL pointer\n");

	if (ptr == NULL)
		return 0;

#ifdef CVMX_BUILD_FOR_UBOOT
	{
		uint64_t uboot_tlb_ptr_to_phys(void *ptr);

		if (((uint32_t) ptr) < 0x80000000) {
			/* Handle useg (unmapped due to ERL) here */
			return (CAST64(ptr) & 0x7FFFFFFF);
		} else if (((uint32_t) ptr) < 0xC0000000) {
			/* Here we handle KSEG0/KSEG1 _pointers_.  We know we are
			 * dealing with 32 bit only values, so we treat them that way.
			 *  Note that a cvmx_phys_to_ptr(cvmx_ptr_to_phys(X)) will not
			 * return X in this case, but the physical address of the
			 * KSEG0/KSEG1 address.
			 */
			return (CAST64(ptr) & 0x1FFFFFFF);
		} else
			return (uboot_tlb_ptr_to_phys(ptr));	/* Should not get get here in !TLB case */
	}

#endif

#ifdef __linux__
	if (sizeof(void *) == 8) {
		/* We're running in 64 bit mode. Normally this means that we
		 * can use 40 bits of address space (the hardware limit).
		 * Unfortunately there is one case were we need to limit this
		 * to 30 bits, sign extended 32 bit. Although these are 64 bits
		 * wide, only 30 bits can be used
		 */
		if ((CAST64(ptr) >> 62) == 3)
			return CAST64(ptr) & cvmx_build_mask(30);
		else
			return CAST64(ptr) & cvmx_build_mask(42);
	} else {
#ifdef __KERNEL__
		return (long)(ptr) & 0x1fffffff;
#else
		extern uint64_t linux_mem32_offset;
		if (cvmx_likely(ptr))
			return CAST64(ptr) - linux_mem32_offset;
		else
			return 0;
#endif
	}
#elif defined(_WRS_KERNEL)
	return (long)(ptr) & 0x7fffffff;
#elif defined(VXWORKS_USER_MAPPINGS)
	/* This mapping mode is used in vxWorks 5.5 to support 2GB of ram. The
	   2nd 256MB is mapped at 0x10000000 and the rest of memory is 1:1 */
	uint64_t address = (long)ptr;
	if (address & 0x80000000)
		return address & 0x1fffffff;	/* KSEG pointers directly map the lower 256MB and bootbus */
	else if ((address >= 0x10000000) && (address < 0x20000000))
		return address + 0x400000000ull;	/* 256MB-512MB is a virtual mapping for the 2nd 256MB */
	else
		return address;	/* Looks to be a 1:1 mapped userspace pointer */
#else
# if CVMX_USE_1_TO_1_TLB_MAPPINGS
	/* We are assumung we're running the Simple Executive standalone.  In
	 * this mode the TLB is setup to perform 1:1 mapping and 32 bit sign
	 * extended addresses are never used.  Since we know all this, save the
	 * masking cycles and do nothing.
	 *
	 * NOTE: This does the wrong thing in many cases and should only be used
	 * for memory allocated using cvmx_bootmem_alloc.  Also, memory isn't
	 * really 1:1 mapped.
	 */
	if (CVMX_ENABLE_PARAMETER_CHECKING) {
		extern uint64_t __cvmx_max_1_to_1_mapped;
		cvmx_warn_if(CAST_ULL(CAST64(ptr)) >  __cvmx_max_1_to_1_mapped,
			"cvmx_ptr_to_phys() pointer out of range\n");
	}

	return CAST64(ptr);
# else
	if (sizeof(void *) == 8) {
		/* Largest PABITS is 42 for CN78XX */
		if ((CAST64(ptr) >> 62) == 3)
			/* 32-bit compatibility address space */
			return CAST64(ptr) & cvmx_build_mask(30);
		else
			/* Assuming VA[62..63] == 0b10 := XKPHYS */
			return CAST64(ptr) & cvmx_build_mask(42);
		/* FIXME- many cases not handled here */
	} else
		return (long)(ptr) & 0x7fffffff;

# endif
#endif
}

/**
 * Convert a memory pointer (void*) into a hardware compatable
 * memory address (uint64_t). Octeon hardware widgets don't
 * understand logical addresses.
 *
 * @param ptr    C style memory pointer
 * @return Hardware physical address
 *
 * NOTE: For simple executive applications this should only be used for pointers
 * allocated using cvmx_bootmem_alloc.  Using this on pointers to global
 * variables or memory allocated on the stack may return incorrect values.
 * This function is much faster than cvmx_ptr_to_phys2() due to this assumption.
 */
#ifndef __OPTIMIZE_SIZE__
static inline __attribute__ ((__always_inline__))
uint64_t cvmx_ptr_to_phys(void *ptr)
{
	return __cvmx_ptr_to_phys_internal__(ptr);
}
#else
uint64_t cvmx_ptr_to_phys(void *ptr);
#endif

/**
 * Convert a memory pointer (void*) into a hardware compatable
 * memory address (uint64_t). Octeon hardware widgets don't
 * understand logical addresses.
 *
 * @param ptr    C style memory pointer
 * @return Hardware physical address
 *
 * NOTE: This function should return the correct value in all cases.
 * NOTE: Internal use only
 */
static inline __attribute__ ((__always_inline__))
uint64_t __cvmx_ptr_to_phys2_internal__(void *ptr)
{
	uint64_t eh_val;
	uint64_t lo;
	uint32_t pagemask;
	uint64_t asid;
	unsigned long offset;
	int index;

	if (CVMX_ENABLE_PARAMETER_CHECKING)
		cvmx_warn_if(ptr == NULL, "cvmx_ptr_to_phys() passed a NULL pointer\n");

	if (ptr == NULL)
		return 0;

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
}

/**
 * Convert a memory pointer (void *) into a hardware compatible memory address
 * (uint64_t).  Octeon hardware widgets don't understand logical addresses.
 *
 * @param ptr	C style memory pointer
 * @return	Hardware physical address
 *
 * NOTE: This function should return the correct value in all cases.
 */
#ifndef __OPTIMIZE_SIZE__
static inline __attribute__ ((__always_inline__))
uint64_t cvmx_ptr_to_phys2(void *ptr)
{
	return __cvmx_ptr_to_phys2_internal__(ptr);
}
#else
uint64_t cvmx_ptr_to_phys2(void *ptr);
#endif

/**
 * Convert a hardware physical address (uint64_t) into a
 * memory pointer (void *).
 *
 * @param physical_address
 *               Hardware physical address to memory
 * @return Pointer to memory
 *
 * NOTE: Internal use only
 */
static inline __attribute__ ((__always_inline__))
void *__cvmx_phys_to_ptr_internal__(uint64_t physical_address)
{
	if (CVMX_ENABLE_PARAMETER_CHECKING)
		cvmx_warn_if(physical_address == 0, "cvmx_phys_to_ptr() passed a zero address\n");

#ifdef CVMX_BUILD_FOR_UBOOT

	/* U-boot is a special case, as it is running in 32 bit mode, using the TLB to map code/data
	 ** which can have a physical address above the 32 bit address space.  1-1 mappings are used
	 ** to allow the low 2 GBytes to be accessed as in error level.
	 **
	 ** NOTE:  This conversion can cause problems in u-boot, as users may want to enter addresses
	 ** like 0xBFC00000 (kseg1 boot bus address), which is a valid 64 bit physical address,
	 ** but is likely intended to be a boot bus address. */

	if (physical_address < 0x80000000) {
		/* Handle useg here.  ERL is set, so useg is unmapped.  This is the only physical
		 ** address range that is directly addressable by u-boot. */
		return CASTPTR(void, physical_address);
	} else {
		DECLARE_GLOBAL_DATA_PTR;
		extern char uboot_start;
		/* Above 0x80000000 we can only support one case - a physical address
		 ** that is mapped for u-boot code/data.  We check against the u-boot mem range,
		 ** and return NULL if it is out of this range.
		 */
		if (physical_address >= gd->bd->bi_uboot_ram_addr && physical_address < gd->bd->bi_uboot_ram_addr + gd->bd->bi_uboot_ram_used_size) {
			return ((char *)&uboot_start + (physical_address - gd->bd->bi_uboot_ram_addr));
		} else
			return (NULL);
	}

	if (physical_address >= 0x80000000)
		return NULL;
#endif
#ifdef __linux__
	if (sizeof(void *) == 8) {
		/* Just set the top bit, avoiding any TLB uglyness */
		return CASTPTR(void, CVMX_ADD_SEG(CVMX_MIPS_SPACE_XKPHYS, physical_address));
	} else {
		extern uint64_t linux_mem32_offset;
		if (cvmx_likely(physical_address))
			return CASTPTR(void, physical_address + linux_mem32_offset);
		else
			return NULL;
	}
#elif defined(_WRS_KERNEL)
	return CASTPTR(void, CVMX_ADD_SEG32(CVMX_MIPS32_SPACE_KSEG0, physical_address));
#elif defined(VXWORKS_USER_MAPPINGS)
	/* This mapping mode is used in vxWorks 5.5 to support 2GB of ram. The
	   2nd 256MB is mapped at 0x10000000 and the rest of memory is 1:1 */
	if ((physical_address >= 0x10000000) && (physical_address < 0x20000000))
		return CASTPTR(void, CVMX_ADD_SEG32(CVMX_MIPS32_SPACE_KSEG0, physical_address));
	else if ((OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX))
		 && (physical_address >= 0x410000000ull)
		 && (physical_address < 0x420000000ull))
		return CASTPTR(void, physical_address - 0x400000000ull);
	else
		return CASTPTR(void, physical_address);
#else

#if CVMX_USE_1_TO_1_TLB_MAPPINGS
	/* We are assumung we're running the Simple Executive standalone. In this
	   mode the TLB is setup to perform 1:1 mapping and 32 bit sign extended
	   addresses are never used. Since we know all this, save bit insert
	   cycles and do nothing */
	if (CVMX_ENABLE_PARAMETER_CHECKING) {
		extern uint64_t __cvmx_max_1_to_1_mapped;
		cvmx_warn_if(physical_address >  __cvmx_max_1_to_1_mapped,
			"cvmx_phys_to_ptr() pointer out of range\n");
	}

	return CASTPTR(void, physical_address);
#else
	if (!physical_address)
		return NULL;
	/* Set the XKPHYS/KSEG0 bit as appropriate based on ABI */
	if (sizeof(void *) == 8)
		return CASTPTR(void, CVMX_ADD_SEG(CVMX_MIPS_SPACE_XKPHYS, physical_address));
	else
		return CASTPTR(void, CVMX_ADD_SEG32(CVMX_MIPS32_SPACE_KSEG0, physical_address));

#endif

#endif
}

/**
 * Convert a hardware physical address (uint64_t) into a
 * memory pointer (void *).
 *
 * @param physical_address
 *               Hardware physical address to memory
 * @return Pointer to memory
 *
 * NOTE: this function is defined as non-static inline in
 * U-Boot's lib_octeon.c.
 */
#ifndef __OPTIMIZE_SIZE__
static inline __attribute__ ((__always_inline__))
void *cvmx_phys_to_ptr(uint64_t physical_address)
{
	return __cvmx_phys_to_ptr_internal__(physical_address);
}
#else
void *cvmx_phys_to_ptr(uint64_t physical_address);
#endif

/* The following #if controls the definition of the macro
    CVMX_BUILD_WRITE64. This macro is used to build a store operation to
    a full 64bit address. With a 64bit ABI, this can be done with a simple
    pointer access. 32bit ABIs require more complicated assembly */
#if defined(CVMX_ABI_N64) || defined(CVMX_ABI_EABI)

/* We have a full 64bit ABI. Writing to a 64bit address can be done with
    a simple volatile pointer */
#define CVMX_BUILD_WRITE64(TYPE, ST)                                    \
static inline void cvmx_write64_##TYPE(uint64_t addr, TYPE##_t val)     \
{                                                                       \
    *CASTPTR(volatile TYPE##_t, addr) = val;                            \
}

#elif defined(CVMX_ABI_N32)

/* The N32 ABI passes all 64bit quantities in a single register, so it is
    possible to use the arguments directly. We have to use inline assembly
    for the actual store since a pointer would truncate the address */
#define CVMX_BUILD_WRITE64(TYPE, ST)                                    \
static inline void cvmx_write64_##TYPE(uint64_t addr, TYPE##_t val)     \
{                                                                       \
    asm volatile (ST " %[v], 0(%[c])" ::[v] "r" (val), [c] "r" (addr)); \
}

#elif defined(CVMX_ABI_O32)

#ifdef __KERNEL__
#define CVMX_BUILD_WRITE64(TYPE, LT) extern void cvmx_write64_##TYPE(uint64_t csr_addr, TYPE##_t val);
#else

/* Ok, now the ugly stuff starts. O32 splits 64bit quantities into two
    separate registers. Assembly must be used to put them back together
    before they're used. What should be a simple store becomes a
    convoluted mess of shifts and ors */
#define CVMX_BUILD_WRITE64(TYPE, ST)                                    \
static inline void cvmx_write64_##TYPE(uint64_t csr_addr, TYPE##_t val) \
{                                                                       \
    if (sizeof(TYPE##_t) == 8)                                          \
    {                                                                   \
        uint32_t csr_addrh = csr_addr>>32;                              \
        uint32_t csr_addrl = csr_addr;                                  \
        uint32_t valh = (uint64_t)val>>32;                              \
        uint32_t vall = val;                                            \
        uint32_t tmp1;                                                  \
        uint32_t tmp2;                                                  \
        uint32_t tmp3;                                                  \
                                                                        \
        asm volatile (                                                  \
            ".set push\n"                                             \
            ".set mips64\n"                                             \
            "dsll   %[tmp1], %[valh], 32\n"                             \
            "dsll   %[tmp2], %[csrh], 32\n"                             \
            "dsll   %[tmp3], %[vall], 32\n"                             \
            "dsrl   %[tmp3], %[tmp3], 32\n"                             \
            "or     %[tmp1], %[tmp1], %[tmp3]\n"                        \
            "dsll   %[tmp3], %[csrl], 32\n"                             \
            "dsrl   %[tmp3], %[tmp3], 32\n"                             \
            "or     %[tmp2], %[tmp2], %[tmp3]\n"                        \
            ST "    %[tmp1], 0(%[tmp2])\n"                              \
            ".set pop\n"                                             \
            : [tmp1] "=&r" (tmp1), [tmp2] "=&r" (tmp2), [tmp3] "=&r" (tmp3)\
            : [valh] "r" (valh), [vall] "r" (vall),                     \
              [csrh] "r" (csr_addrh), [csrl] "r" (csr_addrl)            \
        );                                                              \
    }                                                                   \
    else                                                                \
    {                                                                   \
        uint32_t csr_addrh = csr_addr>>32;                              \
        uint32_t csr_addrl = csr_addr;                                  \
        uint32_t tmp1;                                                  \
        uint32_t tmp2;                                                  \
                                                                        \
        asm volatile (                                                  \
            ".set push\n"                                             \
            ".set mips64\n"                                             \
            "dsll   %[tmp1], %[csrh], 32\n"                             \
            "dsll   %[tmp2], %[csrl], 32\n"                             \
            "dsrl   %[tmp2], %[tmp2], 32\n"                             \
            "or     %[tmp1], %[tmp1], %[tmp2]\n"                        \
            ST "    %[val], 0(%[tmp1])\n"                               \
            ".set pop\n"                                             \
            : [tmp1] "=&r" (tmp1), [tmp2] "=&r" (tmp2)                  \
            : [val] "r" (val), [csrh] "r" (csr_addrh),                  \
              [csrl] "r" (csr_addrl)                                    \
        );                                                              \
    }                                                                   \
}

#endif

#else

/* cvmx-abi.h didn't recognize the ABI. Force the compile to fail. */
#error: Unsupported ABI

#endif

/* The following #if controls the definition of the macro
    CVMX_BUILD_READ64. This macro is used to build a load operation from
    a full 64bit address. With a 64bit ABI, this can be done with a simple
    pointer access. 32bit ABIs require more complicated assembly */
#if defined(CVMX_ABI_N64) || defined(CVMX_ABI_EABI)

/* We have a full 64bit ABI. Writing to a 64bit address can be done with
    a simple volatile pointer */
#define CVMX_BUILD_READ64(TYPE, LT)                                     \
static inline TYPE##_t cvmx_read64_##TYPE(uint64_t addr)                \
{                                                                       \
    return *CASTPTR(volatile TYPE##_t, addr);                           \
}

#elif defined(CVMX_ABI_N32)

/* The N32 ABI passes all 64bit quantities in a single register, so it is
    possible to use the arguments directly. We have to use inline assembly
    for the actual store since a pointer would truncate the address */
#define CVMX_BUILD_READ64(TYPE, LT)                                     \
static inline TYPE##_t cvmx_read64_##TYPE(uint64_t addr)                \
{                                                                       \
    TYPE##_t val;                                                       \
    asm volatile (LT " %[v], 0(%[c])": [v] "=r" (val) : [c] "r" (addr));\
    return val;                                                         \
}

#elif defined(CVMX_ABI_O32)

#ifdef __KERNEL__
#define CVMX_BUILD_READ64(TYPE, LT) extern TYPE##_t cvmx_read64_##TYPE(uint64_t csr_addr);
#else

/* Ok, now the ugly stuff starts. O32 splits 64bit quantities into two
    separate registers. Assembly must be used to put them back together
    before they're used. What should be a simple load becomes a
    convoluted mess of shifts and ors */
#define CVMX_BUILD_READ64(TYPE, LT)                                     \
static inline TYPE##_t cvmx_read64_##TYPE(uint64_t csr_addr)            \
{                                                                       \
    if (sizeof(TYPE##_t) == 8)                                          \
    {                                                                   \
        uint32_t csr_addrh = csr_addr>>32;                              \
        uint32_t csr_addrl = csr_addr;                                  \
        uint32_t valh;                                                  \
        uint32_t vall;                                                  \
                                                                        \
        asm volatile (                                                  \
            ".set push\n"                                               \
            ".set mips64\n"                                             \
            "dsll   %[valh], %[csrh], 32\n"                             \
            "dsll   %[vall], %[csrl], 32\n"                             \
            "dsrl   %[vall], %[vall], 32\n"                             \
            "or     %[valh], %[valh], %[vall]\n"                        \
            LT "    %[vall], 0(%[valh])\n"                              \
            "dsrl   %[valh], %[vall], 32\n"                             \
            "sll    %[vall], 0\n"                                       \
            "sll    %[valh], 0\n"                                       \
            ".set pop\n"                                                \
            : [valh] "=&r" (valh), [vall] "=&r" (vall)                  \
            : [csrh] "r" (csr_addrh), [csrl] "r" (csr_addrl)            \
        );                                                              \
        return ((uint64_t)valh<<32) | vall;                             \
    }                                                                   \
    else                                                                \
    {                                                                   \
        uint32_t csr_addrh = csr_addr>>32;                              \
        uint32_t csr_addrl = csr_addr;                                  \
        TYPE##_t val;                                                   \
        uint32_t tmp;                                                   \
                                                                        \
        asm volatile (                                                  \
            ".set push\n"                                             \
            ".set mips64\n"                                             \
            "dsll   %[val], %[csrh], 32\n"                              \
            "dsll   %[tmp], %[csrl], 32\n"                              \
            "dsrl   %[tmp], %[tmp], 32\n"                               \
            "or     %[val], %[val], %[tmp]\n"                           \
            LT "    %[val], 0(%[val])\n"                                \
            ".set pop\n"                                             \
            : [val] "=&r" (val), [tmp] "=&r" (tmp)                      \
            : [csrh] "r" (csr_addrh), [csrl] "r" (csr_addrl)            \
        );                                                              \
        return val;                                                     \
    }                                                                   \
}

#endif /* __KERNEL__ */

#else

/* cvmx-abi.h didn't recognize the ABI. Force the compile to fail. */
#error: Unsupported ABI

#endif

/* The following defines 8 functions for writing to a 64bit address. Each
    takes two arguments, the address and the value to write.
    cvmx_write64_int64      cvmx_write64_uint64
    cvmx_write64_int32      cvmx_write64_uint32
    cvmx_write64_int16      cvmx_write64_uint16
    cvmx_write64_int8       cvmx_write64_uint8 */
CVMX_BUILD_WRITE64(int64, "sd");
CVMX_BUILD_WRITE64(int32, "sw");
CVMX_BUILD_WRITE64(int16, "sh");
CVMX_BUILD_WRITE64(int8, "sb");
CVMX_BUILD_WRITE64(uint64, "sd");
CVMX_BUILD_WRITE64(uint32, "sw");
CVMX_BUILD_WRITE64(uint16, "sh");
CVMX_BUILD_WRITE64(uint8, "sb");

/* The following defines 8 functions for reading from a 64bit address. Each
    takes the address as the only argument
    cvmx_read64_int64       cvmx_read64_uint64
    cvmx_read64_int32       cvmx_read64_uint32
    cvmx_read64_int16       cvmx_read64_uint16
    cvmx_read64_int8        cvmx_read64_uint8 */
CVMX_BUILD_READ64(int64, "ld");
CVMX_BUILD_READ64(int32, "lw");
CVMX_BUILD_READ64(int16, "lh");
CVMX_BUILD_READ64(int8, "lb");
CVMX_BUILD_READ64(uint64, "ld");
CVMX_BUILD_READ64(uint32, "lw");
CVMX_BUILD_READ64(uint16, "lhu");
CVMX_BUILD_READ64(uint8, "lbu");

/**
 * Write to a CSR address on the specified node
 *
 * @param node		OCTEON node number
 * @param csr_addr	address to write to
 * @param val		value to write
 *
 * NOTE: If the NODE is already encoded in csr_addr then the node in the address
 * will be overwritten with the node parameter.
 */
static inline void cvmx_write_csr_node(uint64_t node, uint64_t csr_addr,
				       uint64_t val)
{
	uint64_t node_addr;
	uint64_t composite_csr_addr;

	node_addr = (node & CVMX_NODE_MASK) << CVMX_NODE_IO_SHIFT;
	composite_csr_addr = (csr_addr & ~CVMX_NODE_IO_MASK) | node_addr;

	cvmx_write64_uint64(composite_csr_addr, val);
	/*
	 * Perform an immediate read after every write to an RSL
	 * register to force the write to complete. It doesn't matter
	 * what RSL read we do, so we choose CVMX_MIO_BOOT_BIST_STAT
	 * on the same node because it is fast and harmless
	 */
	if (((csr_addr >> 40) & 0x7ffff) == (0x118)) {
		cvmx_read64_uint64(CVMX_MIO_BOOT_BIST_STAT | node_addr);
	}
}

/**
 * Write to a CSR address
 *
 * @param csr_addr	address to write to
 * @param val		value to write
 */
static inline void cvmx_write_csr(uint64_t csr_addr, uint64_t val)
{
	cvmx_write64_uint64(csr_addr, val);

	/* Perform an immediate read after every write to an RSL register to force
	   the write to complete. It doesn't matter what RSL read we do, so we
	   choose CVMX_MIO_BOOT_BIST_STAT because it is fast and harmless */
	if (((csr_addr >> 40) & 0x7ffff) == (0x118))
		cvmx_read64_uint64(CVMX_MIO_BOOT_BIST_STAT);
}

/**
 * Perform a 64-bit write to an IO address
 *
 * @param io_addr	I/O address to write to
 * @param val		64-bit value to write
 */
static inline void cvmx_write_io(uint64_t io_addr, uint64_t val)
{
	cvmx_write64_uint64(io_addr, val);
}

/**
 * Read from a CSR address on the specified node
 *
 * @param node		node to read from
 * @param csr_addr	address of CSR to read from
 *
 * @returns value of CSR address
 *
 * NOTE: If the NODE is already encoded in csr_addr then the node in the address
 * will be overwritten with the node parameter.
 */
static inline uint64_t cvmx_read_csr_node(uint64_t node, uint64_t csr_addr)
{
	uint64_t node_addr;

	node_addr = (csr_addr & ~CVMX_NODE_IO_MASK) |
				(node & CVMX_NODE_MASK) << CVMX_NODE_IO_SHIFT;
	return cvmx_read_csr(node_addr);
}

/**
 * Read from a CSR address
 *
 * @param csr_addr	address of CSR to read from
 *
 * @returns value of CSR address
 */
static inline uint64_t cvmx_read_csr(uint64_t csr_addr)
{
	return cvmx_read64_uint64(csr_addr);
}

static inline void cvmx_send_single(uint64_t data)
{
	cvmx_write64_uint64(CVMX_IOBDMA_ORDERED_IO_ADDR, data);
}

/**
 * Perform a background I/O read operation
 *
 * @param node		node to read from
 * @param scraddr	Starting location where the core puts the result
 * 			0xFFFFFFFFFFFF8000 + (8 * srcaddr)
 * @param csr_addr	Address of CSR to read
 */
static inline void cvmx_read_csr_async_node(uint8_t node, uint64_t scraddr,
					    uint64_t csr_addr)
{
	uint64_t node_addr;
	if (!octeon_has_feature(OCTEON_FEATURE_MULTINODE)) {
		//printf("Resetting node(%d) to 0\n", node);
		node = 0;
	}
	node_addr = csr_addr | (((uint64_t)node & CVMX_NODE_MASK) << CVMX_NODE_IO_SHIFT);
	cvmx_read_csr_async(scraddr, node_addr);
}

/**
 * Perform a background I/O read operation
 *
 * @param scraddr	Starting location where the core puts the result
 * 			0xFFFFFFFFFFFF8000 + (8 * srcaddr)
 * @param csr_addr	Address of CSR to read
 */
static inline void cvmx_read_csr_async(uint64_t scraddr, uint64_t csr_addr)
{
	union {
		uint64_t u64;
		struct {
			uint64_t scraddr:8;
			uint64_t len:8;
			uint64_t addr:48;
		} s;
	} addr;
	addr.u64 = csr_addr;
	addr.s.scraddr = scraddr >> 3;
	addr.s.len = 1;
	cvmx_send_single(addr.u64);
}

/**
 * Number of the Core on which the program is currently running.
 *
 * @return core number
 */
static inline unsigned int cvmx_get_core_num(void)
{
	unsigned int core_num;
	CVMX_RDHWRNV(core_num, 0);
	return core_num;
}

/**
 * Number of the node on which the program is currently running.
 *
 * @return current node number
 */
static inline unsigned int cvmx_get_node_num(void)
{
	unsigned int core_num;
	CVMX_RDHWRNV(core_num, 0);
	return (core_num >> CVMX_NODE_NO_SHIFT) & CVMX_NODE_MASK;
}

/**
 * Node-local number of the core on which the program is currently running.
 *
 * @return core number on local node
 */
static inline unsigned int cvmx_get_local_core_num(void)
{
	unsigned int core_num, core_mask;
	CVMX_RDHWRNV(core_num, 0);
	/* note that MAX_CORES may not be power of 2 */
	core_mask = (1 << CVMX_NODE_NO_SHIFT) -1;
	return core_num & core_mask;
}

/**
 * Returns the number of bits set in the provided value.
 * Simple wrapper for POP instruction.
 *
 * @param val    32 bit value to count set bits in
 *
 * @return Number of bits set
 */
static inline uint32_t cvmx_pop(uint32_t val)
{
	uint32_t pop;
	CVMX_POP(pop, val);
	return pop;
}

/**
 * Returns the number of bits set in the provided value.
 * Simple wrapper for DPOP instruction.
 *
 * @param val    64 bit value to count set bits in
 *
 * @return Number of bits set
 */
static inline int cvmx_dpop(uint64_t val)
{
	int pop;
	CVMX_DPOP(pop, val);
	return pop;
}

/**
 * @deprecated
 * Provide current cycle counter as a return value. Deprecated, use
 * cvmx_clock_get_count(CVMX_CLOCK_CORE) to get cycle counter.
 *
 * @return current cycle counter
 */
static inline uint64_t cvmx_get_cycle(void)
{
	return cvmx_clock_get_count(CVMX_CLOCK_CORE);
}

/**
 * @deprecated
 * Reads a chip global cycle counter.  This counts SCLK cycles since
 * chip reset.  The counter is 64 bit. This function is deprecated as the rate
 * of the global cycle counter is different between Octeon+ and Octeon2, use
 * cvmx_clock_get_count(CVMX_CLOCK_SCLK) instead. For Octeon2, the clock rate
 * of SCLK may be differnet than the core clock.
 *
 * @return Global chip cycle count since chip reset.
 */
static inline uint64_t cvmx_get_cycle_global(void)
{
	return cvmx_clock_get_count(CVMX_CLOCK_IPD);
}
static inline uint64_t cvmx_get_cycle_global(void) __attribute__((deprecated));

/**
 * Wait for the specified number of core clock cycles
 *
 * @param cycles
 */
static inline void cvmx_wait(uint64_t cycles)
{
	uint64_t done = cvmx_get_cycle() + cycles;

	while (cvmx_get_cycle() < done) {
		/* Spin */
	}
}

static inline void __cvmx_wait_usec_internal__(uint64_t usec)
{
	uint64_t done = cvmx_get_cycle() + usec * cvmx_clock_get_rate(CVMX_CLOCK_CORE) / 1000000;
	while (cvmx_get_cycle() < done) {
		/* Spin */
	}
}

/**
 * Wait for the specified number of micro seconds
 *
 * @param usec   micro seconds to wait
 *
 * NOTE: this function is defined as non-static inline in
 * U-Boot's lib_octeon.c.
 */
#ifndef __OPTIMIZE_SIZE__
static inline void cvmx_wait_usec(uint64_t usec)
{
	__cvmx_wait_usec_internal__(usec);
}
#else
void cvmx_wait_usec(uint64_t usec);
#endif

/**
 * Wait for the specified number of io clock cycles
 *
 * @param cycles
 */
static inline void cvmx_wait_io(uint64_t cycles)
{
	uint64_t done = cvmx_clock_get_count(CVMX_CLOCK_SCLK) + cycles;

	while (cvmx_clock_get_count(CVMX_CLOCK_SCLK) < done) {
		/* Spin */
	}
}

/**
 * Perform a soft reset of Octeon
 *
 * @return
 */
static inline void cvmx_reset_octeon_node(uint8_t node)
{
	cvmx_ciu_soft_rst_t ciu_soft_rst;
	ciu_soft_rst.u64 = 0;
	ciu_soft_rst.s.soft_rst = 1;
	if (OCTEON_IS_OCTEON3())
		cvmx_write_csr_node(node, CVMX_RST_SOFT_RST, ciu_soft_rst.u64);
	else
		cvmx_write_csr_node(node, CVMX_CIU_SOFT_RST, ciu_soft_rst.u64);
}

/**
 * Perform a soft reset of Octeon
 *
 * @return
 */
static inline void cvmx_reset_octeon(void)
{
	cvmx_ciu_soft_rst_t ciu_soft_rst;
	ciu_soft_rst.u64 = 0;
	ciu_soft_rst.s.soft_rst = 1;
	if (OCTEON_IS_OCTEON3())
		cvmx_write_csr(CVMX_RST_SOFT_RST, ciu_soft_rst.u64);
	else
		cvmx_write_csr(CVMX_CIU_SOFT_RST, ciu_soft_rst.u64);
}

/**
 * Read a byte of fuse data
 * @param node		node to read from
 * @param byte_addr	address to read
 *
 * @return fuse value: 0 or 1
 */
static inline uint8_t cvmx_fuse_read_byte_node(uint8_t node, int byte_addr)
{
	cvmx_mio_fus_rcmd_t read_cmd;

	read_cmd.u64 = 0;
	read_cmd.s.addr = byte_addr;
	read_cmd.s.pend = 1;
	cvmx_write_csr_node(node, CVMX_MIO_FUS_RCMD, read_cmd.u64);
	while ((read_cmd.u64 = cvmx_read_csr_node(node, CVMX_MIO_FUS_RCMD))
	       && read_cmd.s.pend)
		;
	return read_cmd.s.dat;
}

/**
 * Read a byte of fuse data
 * @param byte_addr   address to read
 *
 * @return fuse value: 0 or 1
 */
static inline uint8_t cvmx_fuse_read_byte(int byte_addr)
{
	cvmx_mio_fus_rcmd_t read_cmd;

	read_cmd.u64 = 0;
	read_cmd.s.addr = byte_addr;
	read_cmd.s.pend = 1;
	cvmx_write_csr(CVMX_MIO_FUS_RCMD, read_cmd.u64);
	while ((read_cmd.u64 = cvmx_read_csr(CVMX_MIO_FUS_RCMD))
	       && read_cmd.s.pend)
		;
	return read_cmd.s.dat;
}

/**
 * Read a single fuse bit
 *
 * @param node   Node number
 * @param fuse   Fuse number (0-1024)
 *
 * @return fuse value: 0 or 1
 */
static inline int cvmx_fuse_read_node(uint8_t node, int fuse)
{
	return (cvmx_fuse_read_byte_node(node, fuse >> 3) >> (fuse & 0x7)) & 1;
}

/**
 * Read a single fuse bit
 *
 * @param fuse   Fuse number (0-1024)
 *
 * @return fuse value: 0 or 1
 */
static inline int cvmx_fuse_read(int fuse)
{
	return (cvmx_fuse_read_byte(fuse >> 3) >> (fuse & 0x7)) & 1;
}

static inline uint64_t cvmx_mask_to_localaddr(uint64_t addr)
{
	return (addr & 0xffffffffff);
}

static inline uint64_t cvmx_addr_on_node(uint64_t node, uint64_t addr)
{
	return (node << 40) | cvmx_mask_to_localaddr(addr);
}

/**
 * Given a CSR address return the node number of that address
 *
 * @param addr	Address to extract node number from
 *
 * @return node number
 */
static inline uint8_t cvmx_csr_addr_to_node(uint64_t addr)
{
	return (addr >> CVMX_NODE_IO_SHIFT) & CVMX_NODE_MASK;
}

/**
 * Strip the node address bits from a CSR address
 *
 * @param addr	CSR address to strip the node bits from
 *
 * @return CSR address with the node bits set to zero
 */
static inline uint64_t cvmx_csr_addr_strip_node(uint64_t addr)
{
	return addr & ~((uint64_t)CVMX_NODE_MASK << CVMX_NODE_IO_SHIFT);
}
#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif /* __CVMX_ACCESS_NATIVE_H__ */
