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
 * Support functions for managing command queues used for
 * various hardware blocks.
 *
 * The common command queue infrastructure abstracts out the
 * software necessary for adding to Octeon's chained queue
 * structures. These structures are used for commands to the
 * PKO, ZIP, DFA, RAID, and DMA engine blocks. Although each
 * hardware unit takes commands and CSRs of different types,
 * they all use basic linked command buffers to store the
 * pending request. In general, users of the CVMX API don't
 * call cvmx-cmd-queue functions directly. Instead the hardware
 * unit specific wrapper should be used. The wrappers perform
 * unit specific validation and CSR writes to submit the
 * commands.
 *
 * Even though most software will never directly interact with
 * cvmx-cmd-queue, knowledge of its internal workings can help
 * in diagnosing performance problems and help with debugging.
 *
 * Command queue pointers are stored in a global named block
 * called "cvmx_cmd_queues". Except for the PKO queues, each
 * hardware queue is stored in its own cache line to reduce SMP
 * contention on spin locks. The PKO queues are stored such that
 * every 16th queue is next to each other in memory. This scheme
 * allows for queues being in separate cache lines when there
 * are low number of queues per port. With 16 queues per port,
 * the first queue for each port is in the same cache area. The
 * second queues for each port are in another area, etc. This
 * allows software to implement very efficient lockless PKO with
 * 16 queues per port using a minimum of cache lines per core.
 * All queues for a given core will be isolated in the same
 * cache area.
 *
 * <hr> $Revision: 90195 $ <hr>
 */

#ifndef __OCTEON_CMD_QUEUE_H__
#define __OCTEON_CMD_QUEUE_H__

#include "octeon-fpa.h"

#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Enumeration representing all hardware blocks that use command
 * queues. Each hardware block has up to 65536 sub identifiers for
 * multiple command queues. Not all chips support all hardware
 * units.
 */
typedef enum {
	CVMX_CMD_QUEUE_PKO_BASE = 0x00,
#define CVMX_CMD_QUEUE_PKO(queue) ((cvmx_cmd_queue_id_t)(CVMX_CMD_QUEUE_PKO_BASE + (0xffff&(queue))))
	CVMX_CMD_QUEUE_ZIP = 0x10,
#define CVMX_CMD_QUEUE_ZIP_QUE(queue) ((cvmx_cmd_queue_id_t)(CVMX_CMD_QUEUE_ZIP + (0xffff&(queue))))
	CVMX_CMD_QUEUE_DFA = 0x20,
	CVMX_CMD_QUEUE_RAID = 0x30,
	CVMX_CMD_QUEUE_DMA_BASE = 0x40,
#define CVMX_CMD_QUEUE_DMA(queue) ((cvmx_cmd_queue_id_t)(CVMX_CMD_QUEUE_DMA_BASE + (0xffff&(queue))))
	CVMX_CMD_QUEUE_BCH = 0x50,
#define CVMX_CMD_QUEUE_BCH(queue) ((cvmx_cmd_queue_id_t)(CVMX_CMD_QUEUE_BCH + (0xffff&(queue))))
	CVMX_CMD_QUEUE_END = 0x60,
} cvmx_cmd_queue_id_t;

/**
 * Command write operations can fail if the command queue needs
 * a new buffer and the associated FPA pool is empty. It can also
 * fail if the number of queued command words reaches the maximum
 * set at initialization.
 */
typedef enum {
	CVMX_CMD_QUEUE_SUCCESS = 0,
	CVMX_CMD_QUEUE_NO_MEMORY = -1,
	CVMX_CMD_QUEUE_FULL = -2,
	CVMX_CMD_QUEUE_INVALID_PARAM = -3,
	CVMX_CMD_QUEUE_ALREADY_SETUP = -4,
} cvmx_cmd_queue_result_t;

typedef struct {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t now_serving;	    /**< You have lock when this is your ticket */
	uint32_t max_depth;	    /**< Maximum outstanding command words */

	uint64_t fpa_pool:3;	    /**< FPA pool buffers come from */
	uint64_t base_ptr_div128:33;
				    /**< Top of command buffer pointer shifted 7 */
	uint64_t unused2:2;
	uint64_t pool_size_m1:13;
				    /**< FPA buffer size in 64bit words minus 1 */
	uint64_t index:13;	    /**< Number of commands already used in buffer */
#else
	uint32_t max_depth;
	uint32_t now_serving;

	uint64_t index:13;
	uint64_t pool_size_m1:13;
	uint64_t unused2:2;
	uint64_t base_ptr_div128:33;
	uint64_t fpa_pool:3;
#endif
} __cvmx_cmd_queue_state_t;

/**
 * This structure contains the global state of all command queues.
 * It is stored in a bootmem named block and shared by all
 * applications running on Octeon. Tickets are stored in a different
 * cache line that queue information to reduce the contention on the
 * ll/sc used to get a ticket. If this is not the case, the update
 * of queue state causes the ll/sc to fail quite often.
 */
typedef struct {
	uint64_t ticket[(CVMX_CMD_QUEUE_END >> 4) * 16];
	__cvmx_cmd_queue_state_t state[(CVMX_CMD_QUEUE_END >> 4) * 16];
} __cvmx_cmd_queue_all_state_t;

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
						    int pool_size);

/**
 * Shutdown a queue a free it's command buffers to the FPA. The
 * hardware connected to the queue must be stopped before this
 * function is called.
 *
 * @param queue_id Queue to shutdown
 *
 * @return CVMX_CMD_QUEUE_SUCCESS or a failure code
 */
cvmx_cmd_queue_result_t octeon_cmd_queue_shutdown(cvmx_cmd_queue_id_t queue_id);

/**
 * Return the number of command words pending in the queue. This
 * function may be relatively slow for some hardware units.
 *
 * @param queue_id Hardware command queue to query
 *
 * @return Number of outstanding commands
 */
int octeon_cmd_queue_length(cvmx_cmd_queue_id_t queue_id);

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
void *octeon_cmd_queue_buffer(cvmx_cmd_queue_id_t queue_id);

/**
 * @INTERNAL
 * Get the index into the state arrays for the supplied queue id.
 *
 * @param queue_id Queue ID to get an index for
 *
 * @return Index into the state arrays
 */
static inline int __octeon_cmd_queue_get_index(cvmx_cmd_queue_id_t queue_id)
{
	/* Warning: This code currently only works with devices that have 256
	 * queues or less.  Devices with more than 16 queues are laid out in
	 * memory to allow cores quick access to every 16th queue. This reduces
	 * cache thrashing when you are running 16 queues per port to support
	 * lockless operation
	 */
	int unit = queue_id >> 4;
	int core = queue_id & 0xf;
	return unit * 16 + core;
}

/**
 * @INTERNAL
 * Get the queue state structure for the given queue id
 *
 * @param queue_id Queue id to get
 *
 * @return Queue structure or NULL on failure
 */
static inline __cvmx_cmd_queue_state_t *
__octeon_cmd_queue_get_state(cvmx_cmd_queue_id_t queue_id)
{
	extern CVMX_SHARED __cvmx_cmd_queue_all_state_t *__cvmx_cmd_queue_state_ptr;
	return &__cvmx_cmd_queue_state_ptr->state[__octeon_cmd_queue_get_index(queue_id)];
}

/**
 * Write an arbitrary number of command words to a command queue.
 * This is a generic function; the fixed number of command word
 * functions yield higher performance.
 *
 * @param queue_id  Hardware command queue to write to
 * @param cmd_count Number of command words to write
 * @param cmds      Array of commands to write
 *
 * @return CVMX_CMD_QUEUE_SUCCESS or a failure code
 */
static inline cvmx_cmd_queue_result_t
octeon_cmd_queue_write(cvmx_cmd_queue_id_t queue_id,
		     int cmd_count, uint64_t * cmds)
{
	__cvmx_cmd_queue_state_t *qptr = __octeon_cmd_queue_get_state(queue_id);

	/* Normally there is plenty of room in the current buffer for the command */
	if (cvmx_likely(qptr->index + cmd_count < qptr->pool_size_m1)) {
		uint64_t *ptr = (uint64_t *) octeon_phys_to_ptr((uint64_t) qptr->base_ptr_div128 << 7);
		ptr += qptr->index;
		qptr->index += cmd_count;
		while (cmd_count--)
			*ptr++ = *cmds++;
	} else {
		uint64_t *ptr;
		int count;
		/* We need a new command buffer. Fail if there isn't one available */
		uint64_t *new_buffer = (uint64_t *) octeon_fpa_alloc(qptr->fpa_pool);
		if (cvmx_unlikely(new_buffer == NULL)) {
			debug("%s: No queue memory\n", __func__);
			return CVMX_CMD_QUEUE_NO_MEMORY;
		}
		ptr = (uint64_t *) octeon_phys_to_ptr((uint64_t) qptr->base_ptr_div128 << 7);
		/* Figure out how many command words will fit in this buffer.
		 * One location will be needed for the next buffer pointer
		 */
		count = qptr->pool_size_m1 - qptr->index;
		ptr += qptr->index;
		cmd_count -= count;
		while (count--)
			*ptr++ = *cmds++;
		*ptr = octeon_ptr_to_phys(new_buffer);
		/* The current buffer is full and has a link to the next buffer.
		 * Time to write the rest of the commands into the new buffer
		 */
		qptr->base_ptr_div128 = *ptr >> 7;
		qptr->index = cmd_count;
		ptr = new_buffer;
		while (cmd_count--)
			*ptr++ = *cmds++;
	}

	return CVMX_CMD_QUEUE_SUCCESS;
}


#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif /* __CVMX_CMD_QUEUE_H__ */
