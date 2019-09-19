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
 * @file queue.c
 *
 * This example implements message queues between cores.  The default message
 * size is 64 bits, and the default queue size is 1 cache line.  Each queue
 * supports a single reader and a single writer.  This implementation
 * prioritizes making memory accesses efficient over other considerations (such
 * as keeping qeueu size a power of 2 to simplify wrapping calculations.)
 * This implementation is an example, and will likely need to be customized
 * to meet the requirements of a specific application.
 *
 * File version info: $Id: queue.c 98164 2014-05-08 23:05:39Z apinski $
 *
 */

#include <stdio.h>
#include <stdlib.h>
 #include <string.h>
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-coremask.h"
#include "cvmx-bootmem.h"
#include "cvmx-sysinfo.h"

/* State changes outside of this code.  Flush stale values from
   registers.  */
#define COMPILER_BARRIER() asm volatile ("" ::: "memory")

/* number of messages to send between cores */
#define NUM_MESSAGES    (500)
/* 15 is max size that fits in 1 cache line.  The queue can be larger, however
** multiple cache lines will be accessed to read from the queue. */
#define EXAMPLE_QUEUE_SIZE  15  
typedef struct 
{
    uint32_t    pad;  
    uint16_t    rd_idx;  /* Points to full entry, unless rd_idx == wr_idx */
    uint16_t    wr_idx;  /* Points to empty entry, wr_idx==rd_idx is empty queue, full queue
                          * leaves one entry empty */
    uint64_t    data[EXAMPLE_QUEUE_SIZE];
}  example_queue_t;
/* Queue structure should be aligned on a cache line boundary
** so that the entire structure fits into one cache line. */



/**
 * Write a message to a queue, assuming only a single
 * writer (no locking/synchronization performed)
 * 
 * Returns immediately whether message was sent or
 * not.
 * 
 * @param q_ptr  pointer to queue structure to write to
 * @param msg    message to write
 * 
 * @return 1 if message written
 *         0 if message not written (queue full)
 */
int queue_write_single_nonblock(example_queue_t *q_ptr, uint64_t msg)
{
    /* Check if room is available in queue.
     * Available entries in queue:
       if (wr_idx >= re_idx)
           available entries = Q_SIZE - 1 - wr_idx + rd_idx
       else
           available entries = rd_idx - wr_idx - 1
    */
    if (((q_ptr->wr_idx >= q_ptr->rd_idx) && (EXAMPLE_QUEUE_SIZE - 1 - q_ptr->wr_idx + q_ptr->rd_idx) > 0)
        || (q_ptr->rd_idx - q_ptr->wr_idx - 1) > 0)
    {
        q_ptr->data[q_ptr->wr_idx] = msg;
#if EXAMPLE_QUEUE_SIZE > 15
        /* This is only required if the queue struture does not fit into one cache line */
        CVMX_SYNCW;  /* Make sure message is visible before wr_idx update */
#endif
        q_ptr->wr_idx = (q_ptr->wr_idx < EXAMPLE_QUEUE_SIZE - 1) ? q_ptr->wr_idx + 1 : 0;
#ifdef FLUSH_QUEUE_WRITES
        /* Enable this syncw to push out the writes to the queue structure.  This may
        ** speed up the writes becoming visible to other cores. */
        CVMX_SYNCW;
#endif
        return(1);

    }
    return(0);

}

/**
 * Write a message to a queue, assuming only a single
 * writer (no locking/synchronization performed)
 * 
 * Blocks until message can be sent.
 * 
 * @param q_ptr  pointer to queue structure to write to
 * @param msg    message to write
 * 
 */
void queue_write_single(example_queue_t *q_ptr, uint64_t msg)
{
    /* Don't return until write has completed */
    while (!queue_write_single_nonblock(q_ptr, msg))
	COMPILER_BARRIER();
}

/**
 * Reads a message from a queue assuming a single
 * reader (no locking/synchronization performed.)
 * Returns immediately whether a message is available
 * or not.
 * 
 * @param q_ptr  pointer to queue to read from
 * @param msg    pointer to location to put message in
 * 
 * @return 1 if message read
 *         0 if no messages available
 */
int queue_read_single_nonblock(example_queue_t *q_ptr, uint64_t *msg)
{
    if (q_ptr->rd_idx != q_ptr->wr_idx)
    {
        *msg = q_ptr->data[q_ptr->rd_idx];
        q_ptr->rd_idx = (q_ptr->rd_idx < EXAMPLE_QUEUE_SIZE - 1) ? q_ptr->rd_idx + 1 : 0;
        return(1);
    }
    return(0);

}

/**
 * Reads a message from a queue assuming a single
 * reader (no locking/synchronization performed.)
 * Does not return until a message is available.
 * 
 * @param q_ptr  pointer to queue to read from
 * @return 64 bit message
 */
uint64_t queue_read_single(example_queue_t *q_ptr)
{
    uint64_t msg;

    while (!queue_read_single_nonblock(q_ptr, &msg))
	COMPILER_BARRIER();
    return(msg);
}

/**
 * Gets a pointer to a named bootmem allocated block,
 * allocating it if necessary.  This function is called
 * by all cores, and they will all get the same address.
 * 
 * @param size   size of block to allocate
 * @param name   name of block
 * 
 * @return Pointer to shared memory (physical address)
 *         NULL on failure
 */
void *get_shared_named_block(uint64_t size, char *name)
{
    void *ptr = cvmx_bootmem_alloc_named(size, 128, name);
    if (!ptr)
    {
        /* Either this core did not allocate it, or the allocation request cannot be
        ** satisfied.  Look up the block, and if that fails, then the allocation 
        ** cannot be satisfied
        **/
        if (cvmx_bootmem_find_named_block(name))
            ptr = cvmx_phys_to_ptr(cvmx_bootmem_find_named_block(name)->base_addr);
    }

    return(ptr);

}
/**
 * Message queue example main.
 * 
 * This example must be run on two cores.
 *
 */
int main()
{
    uint64_t my_msg;
    uint64_t i;
    example_queue_t *queue_ptr;
    uint64_t core;
    cvmx_coremask_t *core_mask;


    cvmx_user_app_init();
    core = cvmx_get_core_num();
    core_mask = &cvmx_sysinfo_get()->core_mask;

    if (2 != cvmx_coremask_get_core_count(core_mask))
    {
        printf("This example must be run on precisely two cores\n");
        return -1;
    }

    /* Allocate memory for the queue from the available physical memory that
    ** is managed by the bootmem allocator.
    */
    queue_ptr = get_shared_named_block(sizeof(example_queue_t), "queue_data");
    if (!queue_ptr)
    {
        printf("ERROR: unable to allocate memory for queue structure.\n");
        exit(-1);
    }
    if (cvmx_is_init_core())
        memset(queue_ptr, 0, sizeof(example_queue_t));

    cvmx_coremask_barrier_sync(core_mask);

    if (cvmx_is_init_core())
    {
        for (i = 0; i < NUM_MESSAGES; i++)
        {
            /* Delay randomly to get various empty/full conditions */
            cvmx_wait(rand()%1000);
            if (!(rand()%27))
                cvmx_wait(80000);

            queue_write_single(queue_ptr, i);
        }
    }
    else
    {


        int fail_cnt = 0;
        for (i = 0; i < NUM_MESSAGES; i++)
        {

            /* Delay randomly to get various empty/full conditions */
            cvmx_wait(rand()%1000);
            if (!(rand()%23))
                cvmx_wait(80000);

            my_msg = queue_read_single(queue_ptr);

            if (i != my_msg)
            {
                printf("ERROR: mismatch in received message: expected: %lld, received: %lld\n", (unsigned long long)i, (unsigned long long)my_msg);
            }
        }

    }

    printf("Queue example complete\n");
    return 0;
}
