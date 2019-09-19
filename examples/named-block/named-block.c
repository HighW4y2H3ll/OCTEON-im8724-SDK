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


#include <stdio.h>
#include <string.h>
#include "cvmx.h"
#include "cvmx-bootmem.h"
#include "cvmx-sysinfo.h"

#define BLOCK_NAME "named-test"
#define BLOCK_SIZE (1ull<<20)
#define ULL unsigned long long


static int named_block_pass_fail_test()
{
    const cvmx_bootmem_named_block_desc_t *block_desc;
    int status;
    void *block_ptr;
    uint64_t wanted_address;

    cvmx_user_app_init();
    if (!cvmx_is_init_core())
        return 0;

    printf("INFO: Size of pointer is %llu bytes\n", (ULL)sizeof(void*));

    block_desc = cvmx_bootmem_find_named_block(BLOCK_NAME);
    if (block_desc)
    {
        status = cvmx_bootmem_free_named(BLOCK_NAME);
        if (status == 0)
        {
            printf("FAIL: cvmx_bootmem_free_named didn't free block %s\n", BLOCK_NAME);
            return -1;
        }
    }

    block_desc = cvmx_bootmem_find_named_block(BLOCK_NAME);
    if (block_desc != NULL)
    {
        printf("FAIL: cvmx_bootmem_find_named_block returned non NULL for block that doesn't exist\n");
        return -1;
    }

    block_ptr = cvmx_bootmem_alloc_named(BLOCK_SIZE, 0x10000, BLOCK_NAME);
    if (block_ptr == NULL)
    {
        printf("FAIL: cvmx_bootmem_alloc_named failed to allocate a block\n");
        return -1;
    }
    if (cvmx_ptr_to_phys(block_ptr) & 0xffff)
    {
        cvmx_bootmem_free_named(BLOCK_NAME);
        printf("FAIL: cvmx_bootmem_alloc_named return unaligned pointer %p\n", block_ptr);
        return -1;
    }

    memset(block_ptr, 0x55, BLOCK_SIZE);

    block_desc = cvmx_bootmem_find_named_block(BLOCK_NAME);
    if (block_desc == NULL)
    {
        cvmx_bootmem_free_named(BLOCK_NAME);
        printf("FAIL: cvmx_bootmem_find_named_block didn't find allocated block\n");
        return -1;
    }
    if (block_desc->size < BLOCK_SIZE)
    {
        cvmx_bootmem_free_named(BLOCK_NAME);
        printf("FAIL: Allocated block is smaller than it should be\n");
        return -1;
    }
    if (block_desc->base_addr != cvmx_ptr_to_phys(block_ptr))
    {
        cvmx_bootmem_free_named(BLOCK_NAME);
        printf("FAIL: Block address=0x%llx doesn't match cvmx_ptr_to_phys(block_ptr=%p)=0x%llx\n",
               (ULL)block_desc->base_addr, block_ptr, (ULL)cvmx_ptr_to_phys(block_ptr));
        return -1;
    }
    if (strcmp(block_desc->name, BLOCK_NAME) != 0)
    {
        cvmx_bootmem_free_named(BLOCK_NAME);
        printf("FAIL: Block name=%s doesn't match allocated name=%s\n",
               block_desc->name, BLOCK_NAME);
        return -1;
    }
    status = cvmx_bootmem_free_named(BLOCK_NAME);
    if (status == 0)
    {
        printf("FAIL: Freeing block %s returned bad status 0x%x\n", BLOCK_NAME, status);
        return -1;
    }

    wanted_address = cvmx_ptr_to_phys(block_ptr);

    block_ptr = cvmx_bootmem_alloc_named_address(BLOCK_SIZE, wanted_address, BLOCK_NAME);
    if (block_ptr == NULL)
    {
        printf("FAIL: cvmx_bootmem_alloc_named_address failed to allocate a block\n");
        return -1;
    }
    if (block_ptr != cvmx_phys_to_ptr(wanted_address))
    {
        printf("FAIL: cvmx_bootmem_alloc_named_address allocated block in the wrong place(%p)\n", block_ptr);
        return -1;
    }
    memset(block_ptr, 0xaa, BLOCK_SIZE);

    status = cvmx_bootmem_free_named(BLOCK_NAME);
    if (status == 0)
    {
        printf("FAIL: Freeing block %s returned bad status 0x%x\n", BLOCK_NAME, status);
        return -1;
    }

    block_ptr = cvmx_bootmem_alloc_named_range(BLOCK_SIZE, wanted_address, wanted_address + BLOCK_SIZE, 0x10000, BLOCK_NAME);
    if (block_ptr == NULL)
    {
        printf("FAIL: cvmx_bootmem_alloc_named_range failed to allocate a block\n");
        return -1;
    }
    if (block_ptr != cvmx_phys_to_ptr(wanted_address))
    {
        printf("FAIL: cvmx_bootmem_alloc_named_range allocated block in the wrong place(%p)\n", block_ptr);
        return -1;
    }

    status = cvmx_bootmem_free_named(BLOCK_NAME);
    if (status == 0)
    {
        printf("FAIL: Freeing block %s returned bad status 0x%x\n", BLOCK_NAME, status);
        return -1;
    }

    printf("PASS: All tests passed\n");
    return 0;
}


int main(int argc, char *argv[])
{
    int ret = 0;
    char* named_block= NULL;

    if (( argc == 2 ) && ( strcmp(argv[1], "-l" ) ==0)) {
        /* -l option, list all named blocks */
        cvmx_bootmem_phy_named_block_print();
	ret = 0;
    }else if (( argc == 3 ) && ( strcmp(argv[1], "-r" ) ==0)) {
        /* -r option, remove a named block */
	if ((named_block = argv[2]) != NULL) {
       	    ret = cvmx_bootmem_phy_named_block_free(named_block,0);
	    if (ret ==0) {
		printf ("Failed to remove named block %s \n", named_block);
	    }else {
		printf ("Named block %s is removed \n", named_block);
	    }
        }
    }
    else {
        /* Run pass/fail test by default */
        ret = named_block_pass_fail_test();
    }
    return ret;
}

