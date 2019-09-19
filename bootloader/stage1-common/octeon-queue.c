/***********************license start***************
 * Copyright (c) 2003-2013  Cavium Inc. (support@cavium.com). All rights
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
 * Support functions for managing command queues used for
 * various hardware blocks.
 *
 * <hr>$Revision: 90763 $<hr>
 */
#undef DEBUG
#include "octeon.h"
#include "octeon-fpa.h"
#include "octeon-queue.h"

#ifdef DEBUG
# define _DEBUG	1
#else
# define _DEBUG 0
#endif

/*
 * Output a debug text when condition "cond" is met. The "cond" should be
 * computed by a preprocessor in the best case, allowing for the best
 * optimization.
 */
#undef debug_cond
#undef debug
#define debug_cond(cond, fmt, args...)	\
	do { if (cond) printf(fmt, ##args); } while (0)

#define debug(fmt, args...)		debug_cond(_DEBUG, fmt, ##args)


/**
 * This application uses this pointer to access the global queue
 * state. It points to a bootmem named block.
 */
static __cvmx_cmd_queue_all_state_t cmd_queue_state
				__attribute__((aligned(128)));
CVMX_SHARED __cvmx_cmd_queue_all_state_t *__cvmx_cmd_queue_state_ptr;

/**
 * @INTERNAL
 * Initialize the Global queue state pointer.
 *
 * @return CVMX_CMD_QUEUE_SUCCESS or a failure code
 */
static cvmx_cmd_queue_result_t __octeon_cmd_queue_init_state_ptr(void)
{
	if (cvmx_likely(__cvmx_cmd_queue_state_ptr))
		return CVMX_CMD_QUEUE_SUCCESS;
	__cvmx_cmd_queue_state_ptr = &cmd_queue_state;

	memset(__cvmx_cmd_queue_state_ptr, 0,
	       sizeof(*__cvmx_cmd_queue_state_ptr));
	return CVMX_CMD_QUEUE_SUCCESS;
}

/**
 * Initialize a command queue for use. The initial FPA buffer is
 * allocated and the hardware unit is configured to point to the
 * new command queue.
 *
 * @param queue_id  Hardware command queue to initialize.
 * @param max_depth Maximum outstanding commands that can be queued.
 * @param fpa_pool  FPA pool the command queues should come from.
 * @param pool_size Size of each buffer in the FPA pool (bytes)
 *
 * @return CVMX_CMD_QUEUE_SUCCESS or a failure code
 */
cvmx_cmd_queue_result_t octeon_cmd_queue_initialize(cvmx_cmd_queue_id_t queue_id,
						    int max_depth, int fpa_pool,
						    int pool_size)
{
	__cvmx_cmd_queue_state_t *qstate;
	cvmx_cmd_queue_result_t result = __octeon_cmd_queue_init_state_ptr();
	if (result != CVMX_CMD_QUEUE_SUCCESS)
		return result;

	debug("%s(0x%x, %d, %d, %d)\n", __func__, queue_id, max_depth,
	      fpa_pool, pool_size);
	qstate = __octeon_cmd_queue_get_state(queue_id);
	if (qstate == NULL)
		return CVMX_CMD_QUEUE_INVALID_PARAM;

	/*
	 * We artificially limit max_depth to 1<<20 words. It is an
	 * arbitrary limit.
	 */
	if ((fpa_pool < 0) || (fpa_pool > 7))
		return CVMX_CMD_QUEUE_INVALID_PARAM;
	if ((pool_size < 128) || (pool_size > 65536))
		return CVMX_CMD_QUEUE_INVALID_PARAM;

	/* See if someone else has already initialized the queue */
	if (qstate->base_ptr_div128) {
		if (max_depth != (int)qstate->max_depth) {
			cvmx_dprintf("ERROR: cvmx_cmd_queue_initialize: Queue already initialized with different max_depth (%d).\n",
				     (int)qstate->max_depth);
			return CVMX_CMD_QUEUE_INVALID_PARAM;
		}
		if (fpa_pool != qstate->fpa_pool) {
			cvmx_dprintf("ERROR: cvmx_cmd_queue_initialize: Queue already initialized with different FPA pool (%u).\n",
				     qstate->fpa_pool);
			return CVMX_CMD_QUEUE_INVALID_PARAM;
		}
		if ((pool_size >> 3) - 1 != qstate->pool_size_m1) {
			cvmx_dprintf("ERROR: cvmx_cmd_queue_initialize: Queue already initialized with different FPA pool size (%u).\n",
				     (qstate->pool_size_m1 + 1) << 3);
			return CVMX_CMD_QUEUE_INVALID_PARAM;
		}
		CVMX_SYNCWS;
		return CVMX_CMD_QUEUE_ALREADY_SETUP;
	} else {
		union cvmx_fpa_ctl_status status;
		void *buffer;

		status.u64 = cvmx_read_csr(CVMX_FPA_CTL_STATUS);
		if (!status.s.enb) {
			cvmx_dprintf("ERROR: cvmx_cmd_queue_initialize:"
				     " FPA is not enabled.\n");
			return CVMX_CMD_QUEUE_NO_MEMORY;
		}
		buffer = octeon_fpa_alloc(fpa_pool);
		if (buffer == NULL) {
			cvmx_dprintf("ERROR: cvmx_cmd_queue_initialize: Unable to allocate initial buffer.\n");
			return CVMX_CMD_QUEUE_NO_MEMORY;
		}
		debug("%s: fpa buffer: %p\n", __func__, buffer);
		memset(qstate, 0, sizeof(*qstate));
		qstate->max_depth = max_depth;
		qstate->fpa_pool = fpa_pool;
		qstate->pool_size_m1 = (pool_size >> 3) - 1;
		qstate->base_ptr_div128 = octeon_ptr_to_phys(buffer) / 128;
		/*
		 * We zeroed the now serving field so we need to also
		 * zero the ticket.
		 */
		__cvmx_cmd_queue_state_ptr->ticket[__octeon_cmd_queue_get_index(queue_id)] = 0;
		debug("%s: queue id: %d, queue index: %d\n", __func__,
		      queue_id, __octeon_cmd_queue_get_index(queue_id));
		CVMX_SYNCWS;
		return CVMX_CMD_QUEUE_SUCCESS;
	}
}

/**
 * Shutdown a queue a free it's command buffers to the FPA. The
 * hardware connected to the queue must be stopped before this
 * function is called.
 *
 * @param queue_id Queue to shutdown
 *
 * @return CVMX_CMD_QUEUE_SUCCESS or a failure code
 */
cvmx_cmd_queue_result_t octeon_cmd_queue_shutdown(cvmx_cmd_queue_id_t queue_id)
{
	__cvmx_cmd_queue_state_t *qptr = __octeon_cmd_queue_get_state(queue_id);
	if (qptr == NULL) {
		cvmx_dprintf("ERROR: cvmx_cmd_queue_shutdown: Unable to get queue information.\n");
		return CVMX_CMD_QUEUE_INVALID_PARAM;
	}

	if (qptr->base_ptr_div128) {
		octeon_fpa_free(octeon_phys_to_ptr((uint64_t) qptr->base_ptr_div128 << 7),
			      qptr->fpa_pool, 0);
		qptr->base_ptr_div128 = 0;
	}

	return CVMX_CMD_QUEUE_SUCCESS;
}

/**
 * Return the command buffer to be written to. The purpose of this
 * function is to allow CVMX routine access to the low level buffer
 * for initial hardware setup. User applications should not call this
 * function directly.
 *
 * @param queue_id Command queue to query
 *
 * @return Command buffer or NULL on failure
 */
void *octeon_cmd_queue_buffer(cvmx_cmd_queue_id_t queue_id)
{
	__cvmx_cmd_queue_state_t *qptr = __octeon_cmd_queue_get_state(queue_id);
	if (qptr && qptr->base_ptr_div128)
		return octeon_phys_to_ptr((uint64_t) qptr->base_ptr_div128 << 7);
	else
		return NULL;
}
