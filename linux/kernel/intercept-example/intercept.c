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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/init.h>
#include <linux/etherdevice.h>
#include <linux/mii.h>
#include <asm/octeon/octeon-ethernet-user.h>

#undef OCTEON_MODEL
#define USE_RUNTIME_MODEL_CHECKS 1

#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-wqe.h>

#define INTERCEPT_PORT  "eth0"
#define SEND_PORT       "eth1"

static struct net_device *input_device = NULL;  /* The device we are going to receive packets on */
static struct net_device *output_device = NULL; /* The device the interceptor sends out on */

/**
 * The intercept callback. This is called for every packet
 * that arrives from the Cavuim ethernet driver for the
 * device we registered on.
 *
 * @param dev    The linux ethernet device the packet is from. This will be
 *               a Cavium ethernet port
 * @param work_queue_entry
 *               Raw work queue entry from the hardware
 * @param skb    Linux skbuff of the packet. Note that this may point to the
 *               same memory as the work queue entry.
 * @return Enumeration telling the ethernet driver what to do
 */
static cvm_oct_callback_result_t intercept_callback(struct net_device *dev, void *work_queue_entry, struct sk_buff *skb)
{
    cvmx_wqe_t *work = work_queue_entry;

    /* Check to see if the packet is a broadcast IP packet */
    if (work->word2.s.is_bcast && !work->word2.s.not_IP)
    {
        /* Just blindly drop all packets larger than 256 bytes */
        if (work->word1.len > 256)
        {
            return CVM_OCT_DROP;
        }
        else
        {
            /* Intercept the packet and send it back out the other port */
            cvm_oct_transmit_qos(output_device, work_queue_entry, 1, 0);
            return CVM_OCT_TAKE_OWNERSHIP_WORK;
        }
    }
    else
    {
        /* Packet is not a broadcast or not IP, just pass it along */
        return CVM_OCT_PASS;
    }
}


/**
 * Module main entry point. This is called when the module is
 * loaded.
 *
 * @return
 */
int __init intercept_init(void)
{
    if (cvm_oct_register_callback == NULL)
    {
        printk("Load octeon-ethernet.ko before this module\n");
        return -1;
    }

    /* Use the register callback to get a pointer to our output device. This
        could be done with a standard kernel call, but then someone might
        try and use a non Cavium ethernet device. That would cause serious
        problems */
    output_device = cvm_oct_register_callback(SEND_PORT, NULL);
    if (output_device == NULL)
    {
        printk("Intercept failed to hook into " SEND_PORT "\n");
        return -1;
    }

    /* Register an intercept callback for the incomming port */
    input_device = cvm_oct_register_callback(INTERCEPT_PORT, intercept_callback);
    if (input_device == NULL)
    {
        printk("Intercept failed to hook into " INTERCEPT_PORT "\n");
        return -1;
    }
    /* All setup is complete, tell the user what is going on */
    printk("\n\nIntercept example installed. All packets comming in " INTERCEPT_PORT " will be\n"
           "monitored. Broadcast IP packets larger than 256 bytes will be dropped,\n"
           "while smaller IP broadcasts will be intercepted and sent out " SEND_PORT "\n\n");

    return 0;
}


/**
 * Called before the module is unloaded
 *
 * @return
 */
void __exit intercept_exit(void)
{
    /* Before we unload, disconnect the intercept callback */
    if (input_device)
        cvm_oct_register_callback(INTERCEPT_PORT, NULL);
}

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Cavium Inc. <support@cavium.com>");
MODULE_DESCRIPTION("Cavium Inc. Packet interceptor example.");
module_init(intercept_init);
module_exit(intercept_exit);

