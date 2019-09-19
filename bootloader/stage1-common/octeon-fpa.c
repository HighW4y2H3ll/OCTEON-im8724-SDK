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
 *
 * Support library for the hardware Free Pool Allocator.
 *
 * <hr>$Revision: 89291 $<hr>
 *
 */
#undef DEBUG
#include "octeon.h"
#include "octeon-fpa.h"

/**
 * Current state of all the pools. Use access functions
 * instead of using it directly.
 */
cvmx_fpa_pool_info_t cvmx_fpa_pool_info[CVMX_MAX_NODES][CVMX_FPA_NUM_POOLS];

/**
 * Setup a FPA pool to control a new block of memory. The
 * buffer pointer must be a virtual address.
 *
 * @param pool       Pool to initialize
 *                   0 <= pool < 8
 * @param name       Constant character string to name this pool.
 *                   String is not copied.
 * @param buffer     Pointer to the block of memory to use. This must be
 *                   accessible by all processors and external hardware.
 * @param block_size Size for each block controlled by the FPA
 * @param num_blocks Number of blocks
 *
 * @return 0 on Success,
 *         -1 on failure
 */
int octeon_fpa_setup_pool(uint64_t pool, const char *name, void *buffer,
			uint64_t block_size, uint64_t num_blocks)
{
	char *ptr;
	uint16_t iter_i, iter_j;

	for (iter_i = 0; iter_i < CVMX_MAX_NODES; iter_i++) {
		for (iter_j = 0; iter_j < CVMX_FPA_NUM_POOLS; iter_j++) {
			if (strncmp(cvmx_fpa_pool_info[iter_i][iter_j].name,
						name,
						CVMX_FPA_POOL_NAME_LEN - 1)
					== 0) {
				cvmx_dprintf("ERROR: cvmx_fpa_setup_pool: "
						"Pool name %s is in use by "
						"fpa pool[%d][%d] \n",
						name, iter_i, iter_j);
				return -1;
			}
		}
	}

	if (strlen(name) > CVMX_FPA_POOL_NAME_LEN - 1) {
		cvmx_dprintf("WARNING: cvmx_fpa_setup_pool: "
			     "name length exceeding limit, truncating.\n");
		strncpy(cvmx_fpa_pool_info[0][pool].name, name,
			CVMX_FPA_POOL_NAME_LEN - 1);
		cvmx_fpa_pool_info[0][pool].
			name[CVMX_FPA_POOL_NAME_LEN - 1] = '\0';
	} else
		strcpy(cvmx_fpa_pool_info[0][pool].name, name);

	cvmx_fpa_pool_info[0][pool].size = block_size;
	cvmx_fpa_pool_info[0][pool].starting_element_count = num_blocks;
	cvmx_fpa_pool_info[0][pool].base = buffer;

	ptr = (char *)buffer;
	while (num_blocks--) {
		octeon_fpa_free(ptr, pool, 0);
		ptr += block_size;
	}

	return 0;
}

/**
 * Shutdown a Memory pool and validate that it had all of
 * the buffers originally placed in it. This should only be
 * called by one processor after all hardware has finished
 * using the pool. Most like you will want to have called
 * cvmx_helper_shutdown_packet_io_global() before this
 * function to make sure all FPA buffers are out of the packet
 * IO hardware.
 *
 * @param pool   Pool to shutdown
 *
 * @return Zero on success
 *         - Positive is count of missing buffers
 *         - Negative is too many buffers or corrupted pointers
 */
uint64_t octeon_fpa_shutdown_pool(uint64_t pool)
{
	int errors = 0;
	int count = 0;
	int expected_count = cvmx_fpa_pool_info[0][pool].starting_element_count;
	uint64_t base = cvmx_ptr_to_phys(cvmx_fpa_pool_info[0][pool].base);
	uint64_t finish = base + cvmx_fpa_pool_info[0][pool].size * expected_count;

	count = 0;
	while (1) {
		uint64_t address;
		void *ptr = octeon_fpa_alloc(pool);
		if (!ptr)
			break;

		address = cvmx_ptr_to_phys(ptr);
		if ((address >= base) && (address < finish) &&
		    (((address - base) % cvmx_fpa_pool_info[0][pool].size) == 0)) {
			count++;
		} else {
			cvmx_dprintf("ERROR: cvmx_fpa_shutdown_pool: Illegal "
				     "address 0x%llx in pool %s(%d)\n",
				     CAST_ULL(address),
				     cvmx_fpa_pool_info[0][pool].name,
				     (int)pool);
			errors++;
		}
	}

	if (count < expected_count) {
		cvmx_dprintf("ERROR: cvmx_fpa_shutdown_pool: Pool %s(%d) "
			     "missing %d buffers\n",
			     cvmx_fpa_pool_info[0][pool].name, (int)pool,
			     expected_count - count);
	} else if (count > expected_count) {
		cvmx_dprintf("ERROR: cvmx_fpa_shutdown_pool: Pool %s(%d) "
			     "had %d duplicate buffers\n",
			     cvmx_fpa_pool_info[0][pool].name, (int)pool,
			     count - expected_count);
	}

	if (errors) {
		cvmx_dprintf("ERROR: cvmx_fpa_shutdown_pool: Pool %s(%d) "
			     "started at 0x%llx, ended at 0x%llx, with a step "
			     "of 0x%x\n", cvmx_fpa_pool_info[0][pool].name,
			     (int)pool, CAST_ULL(base), CAST_ULL(finish),
			     (int)cvmx_fpa_pool_info[0][pool].size);
		return -errors;
	} else
		return expected_count - count;
}

/**
 * Gets the block size of buffer in specified pool
 * @param pool	 Pool to get the block size from
 * @return       Size of buffer in specified pool
 */
uint64_t octeon_fpa_get_block_size(uint64_t pool)
{
	return (cvmx_fpa_pool_info[0][pool].size);

}

/**
 * Initialize global configuration for FPA block.
 */
int octeon_fpa_global_initialize(void)
{
	octeon_fpa_enable();
	return 0;
}
