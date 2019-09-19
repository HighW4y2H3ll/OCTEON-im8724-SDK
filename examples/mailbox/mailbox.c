/***********************license start***************
 * Copyright (c) 2003-2015  Cavium Inc. (support@cavium.com). All rights 
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
 * @file mailbox.c
 *
 * Sample code using the mailbox interrupts to pass messages between cores.  This can
 * easily be adapted to different message sizes.
 *
 * File version info: $Id: mailbox.c 127395 2015-10-30 20:18:39Z ytolstov $
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-atomic.h"
#include "cvmx-mbox.h"
#include "cvmx-spinlock.h"
#include "cvmx-coremask.h"
#include "cvmx-sysinfo.h"


/* Spinlock and core count used to enumerate cores independently of core id so that this program
** will produce the same results when run on N cores no matter which set of N cores
** it is run on.
*/
CVMX_SHARED cvmx_spinlock_t lock = CVMX_SPINLOCK_UNLOCKED_INITIALIZER;
CVMX_SHARED unsigned int core_count = 0;

CVMX_SHARED unsigned int mbox_bit = CVMX_MBOX_BIT_USER_0;

/* Memory where mailbox values are stored. Each core has a dedicated
** location to receive messages from every other core
*/
CVMX_SHARED __attribute__ ((aligned (128))) volatile uint16_t mbox_data[CVMX_MIPS_MAX_CORES /* rx core */][CVMX_MIPS_MAX_CORES /* tx core */];
CVMX_SHARED uint64_t mbox_bits[CVMX_MIPS_MAX_CORES][16];

volatile long message_total = 0;
/**
 * Interrupt handler for mailbox interrupt.  Simply keeps a running
 * total of all messages received.
 *
 * @param irq The interrupt that occurred. 
 * @param registers  CPU registers at time of interrupt
 */
static void mbox_interrupt_handler(struct cvmx_mbox *h, uint64_t registers[32])
{
	int sec;
	uint64_t mbox;
	uint16_t i;
	uint64_t core = cvmx_get_core_num();

	/* Read mailbox interrupt register to see what cores have pending messages for 
	** this core. */
	for (sec = 0; sec < 15; sec++) {
		mbox = cvmx_atomic_fetch_and_bclr64_nosync(&mbox_bits[core][sec], 0);
		for (i = 0; i < 64; i++) {
			if (mbox & (1ull << i)) {
				/* Read and process message from other core */
				message_total += mbox_data[core][i + 64 * sec];
			}
		}
		/* Clear the bits that we processed */
		cvmx_atomic_fetch_and_bclr64_nosync(&mbox_bits[core][sec], mbox);
	}
}

static CVMX_SHARED struct cvmx_mbox mbox_handler = {
	.handler = mbox_interrupt_handler
};

/**
 * Send a 64 bit message using the mailbox interrupts
 * and shared memory.
 * Does not check to see if mailbox is empty, messages may
 * be lost if receiver can't keep up.
 * 
 * @param dest_core destination core number
 * @param data      64 bit message to send
 */
static void send_mbox_message(uint64_t dest_core, uint16_t data)
{
	int core = cvmx_get_core_num();
	int bit = core & 0x3f;
	int sec = core >> 6;
	mbox_data[dest_core][core] = data;
	CVMX_SYNCW;
	cvmx_atomic_fetch_and_bset64_nosync(&mbox_bits[dest_core][sec], 1ull << bit);
	cvmx_mbox_signal(mbox_bit, dest_core);
}

/**
 * Mailbox example main()
 *
 * @return
 */
int main()
{
	cvmx_ciu_intx0_t irq_control;
	uint64_t core;
	uint64_t local_core_count;
	int i;
	long expected_total;

	/* Enumerate the cores based on number of cores running, independently
	** of which physical cores are actually running.
	*/
	cvmx_spinlock_lock(&lock);
	core = core_count++;
	cvmx_spinlock_unlock(&lock);

	cvmx_mbox_initialize(1u << mbox_bit);

	if (cvmx_is_init_core())
		cvmx_mbox_register(mbox_bit, &mbox_handler);

	cvmx_coremask_barrier_sync(&cvmx_sysinfo_get()->core_mask);    

	local_core_count = core_count;

	/* Send message to every core, including ourselves*/
	cvmx_coremask_for_each_core(i, &cvmx_sysinfo_get()->core_mask) {
		send_mbox_message(i, core);
	}

	cvmx_coremask_barrier_sync(&cvmx_sysinfo_get()->core_mask);

	/* Compare total of messages received with expected total value */
	expected_total = 0;
    
	while (local_core_count)
		expected_total +=  --local_core_count;
	if (message_total == expected_total)
		printf("Test PASSED, total: %ld\n", message_total);
	else
		printf("Test FAILED, total: %ld (expected %ld) %u cores\n", message_total, expected_total, core_count);


	return 0;
}
