/***********************license start***************
 * Copyright (c) 2003-2016  Cavium Inc. (support@cavium.com). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
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
 *     derived from this software without specific prior writte
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

#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-pko.h"
#include "cvmx-higig.h"
#include "cvmx-srio.h"
#include "cvmx-ilk.h"

#include "traffic-gen.h"

unsigned short ip_fast_csum(char *iph, unsigned int ihl) __attribute__ ((__noinline__));

unsigned short ip_fast_csum(char *iph, unsigned int ihl)
{
    unsigned int csum = 0;
    register char *arg1 asm ("$4") = iph;
    register unsigned int arg2 asm ("$5") = ihl;

    asm volatile (
        "    .set push                 # ip_fast_csum\n"
        "    cins   $5,$5,2,0x1f       # end pointer offset    \n"
        "    lw     $2,0($4)           # load first word       \n"
        "    lw     $6,4($4)           # load 2nd word         \n"
        "    lw     $3,8($4)           # load 3rd word         \n"
        "    daddu  $8,$4,$5           # end pointer           \n"
        "    lw     $5,12($4)          # load 4th word         \n"
        "    addu   $2,$6,$2           # csum = fist + 2nd     \n"
        "    sltu   $6,$2,$6           # check for carry       \n"
        "    addu   $2,$2,$3           # csum += 3rd           \n"
        "    addu   $2,$2,$6           # csum += carry         \n"
        "    sltu   $3,$2,$3           # check for carry       \n"
        "    addu   $2,$2,$5           # csum += 4th           \n"
        "    addu   $2,$2,$3           # csum += carry         \n"
        "    sltu   $5,$2,$5           # check for carry       \n"
        "    addu   $3,$5,$2           # csum += carry         \n"
        "    daddiu $7,$4,16           # offset to next        \n"
        "2:  lw     $2,0($7)           # load next word        \n"
        "    daddiu $7,$7,4            # offset of next word   \n"
        "    addu   $3,$3,$2           # csum += next_word     \n"
        "    sltu   $2,$3,$2           # check for carry       \n"
        "    bne    $7,$8,2b           # check for end ptr     \n"
        "    addu   $3,$2,$3           # csum += carry         \n"
        "                              # Now fold the csum     \n"
        "    move   $2,$3                                      \n"
        "    sll    $3,$2,0x10                                 \n"
        "    addu   $2,$2,$3                                   \n"
        "    sltu   $3,$2,$3                                   \n"
        "    srl    $2,$2,0x10                                 \n"
        "    addu   $2,$2,$3                                   \n"
        "    xori   $2,$2,0xffff                               \n"
        "    andi   $2,$2,0xffff                               \n"
        "    move   %0,$2                                      \n"
        "    nop                                               \n"
       : "=r" (csum)
       : "r" (arg1), "r" (arg2), "0" (csum)
       : "$2", "$3", "$6", "$7", "$8", "memory");

    return csum;
}

/**
 * Convert a pointer to a MAC address into a 64bit number
 * containing the MAC address. Handle unaligned cases.
 *
 * @param mac    MAC address to read
 *
 * @return MAC address as a 64bit number
 */
static inline uint64_t mac_to_uint64(char *mac)
{
    uint64_t m;
    CVMX_LOADUNA_INT64(m, mac, 0);
    return m>>16;
}

/**
 * Store a 64bit number into a MAC address pointer. Handler the unaligned case.
 *
 * @param m      Mac address
 * @param mac    Place to store it
 */
static inline void uint64_to_mac(uint64_t m, char *mac)
{
    CVMX_STOREUNA_INT32(m >> 16, mac, 0);
    CVMX_STOREUNA_INT16(m & 0xffff, mac, 4);
}

int tgpio_add_txf(int port, tx_task_t ptask)
{
    int i;
    port_setup_t *portx;

    if (!ptask)
        return 0;

    portx = port_setup + port;

    for (i = 0; i < portx->tx_ntasks; i++)
        if (portx->tx_tasks[i] == ptask)
        return 0;

    if (i < MAX_TX_TASKS)
    {
        portx->tx_tasks[i] = ptask;
        portx->tx_ntasks = i + 1;
        return 0;
    }
    return 1; /* TX table is full */
}

int tgpio_del_txf(int port, tx_task_t ptask)
{
    int i, j;
    port_setup_t *portx;

    if (!ptask)
        return 0;

    portx = port_setup + port;
    for (i = 0; i < portx->tx_ntasks; i++)
        if (portx->tx_tasks[i] == ptask)
            break;

    if (i == portx->tx_ntasks)
        return 0;

    for (j = i + 1; j < portx->tx_ntasks; j++)
    {
        portx->tx_tasks[i] = portx->tx_tasks[j];
        i++;
    }
    portx->tx_ntasks--;
    return 0;
}

static int txf_src_port_inc(int port, int pcore)
{
    int begin_ip;
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    begin_ip = get_end_l2(port);
    if (cvmx_unlikely(port_setup[port].src_port_inc))
    {
        int p = *(uint16_t*)(data + begin_ip + 20);
        p += port_setup[port].src_port_inc;
        if (p < port_setup[port].src_port_min)
            p = port_setup[port].src_port_max;
        else if (p > port_setup[port].src_port_max)
            p = port_setup[port].src_port_min;
        *(uint16_t*)(data + begin_ip + 20) = p;
    }
    return 0;
}

static int txf_dest_port_inc(int port, int pcore)
{
    int begin_ip = get_end_l2(port);
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].dest_port_inc))
    {
        int p = *(uint16_t*)(data + begin_ip + 22);
        p += port_setup[port].dest_port_inc;
        if (p < port_setup[port].dest_port_min)
            p = port_setup[port].dest_port_max;
        else if (p > port_setup[port].dest_port_max)
            p = port_setup[port].dest_port_min;
        *(uint16_t*)(data + begin_ip + 22) = p;
    }
    return 0;
}

static int txf_src_ip_inc(int port, int pcore)
{
    int begin_ip = get_end_l2(port);
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].src_ip_inc))
    {
        int64_t p = *(uint32_t*)(data + begin_ip + 12);
        p += port_setup[port].src_ip_inc;
        if (p < port_setup[port].src_ip_min)
            p = port_setup[port].src_ip_max;
        else if (p > port_setup[port].src_ip_max)
            p = port_setup[port].src_ip_min;
        *(uint32_t*)(data + begin_ip + 12) = p;

        /* IP checksum */
        data[begin_ip + 10] = 0;
        data[begin_ip + 11] = 0;
        *(uint16_t*)(data + begin_ip + 10) = ip_fast_csum(data + begin_ip, 5);
    }
    return 0;
}

static int txf_dest_ip_inc(int port, int pcore)
{
    int begin_ip = get_end_l2(port);
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].dest_ip_inc))
    {
        int64_t p = *(uint32_t*)(data + begin_ip + 16);
        p += port_setup[port].dest_ip_inc;
        if (p < port_setup[port].dest_ip_min)
            p = port_setup[port].dest_ip_max;
        else if (p > port_setup[port].dest_ip_max)
            p = port_setup[port].dest_ip_min;
        *(uint32_t*)(data + begin_ip + 16) = p;

        /* IP checksum */
        data[begin_ip + 10] = 0;
        data[begin_ip + 11] = 0;
        *(uint16_t*)(data + begin_ip + 10) = ip_fast_csum(data + begin_ip, 5);
    }
    return 0;
}

static int txf_src_port_inc_v6(int port, int pcore)
{
    int begin_ip = get_end_l2(port);
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].src_port_inc))
    {
        int p = *(uint16_t*)(data + begin_ip + 40);
        p += port_setup[port].src_port_inc;
        if (p < port_setup[port].src_port_min)
            p = port_setup[port].src_port_max;
        else if (p > port_setup[port].src_port_max)
            p = port_setup[port].src_port_min;
        *(uint16_t*)(data + begin_ip + 40) = p;
    }
    return 0;
}

static int txf_dest_port_inc_v6(int port, int pcore)
{
    int begin_ip = get_end_l2(port);
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].dest_port_inc))
    {
        int p = *(uint16_t*)(data + begin_ip + 40 + 2);
        p += port_setup[port].dest_port_inc;
        if (p < port_setup[port].dest_port_min)
            p = port_setup[port].dest_port_max;
        else if (p > port_setup[port].dest_port_max)
            p = port_setup[port].dest_port_min;
        *(uint16_t*)(data + begin_ip + 40 + 2) = p;
    }
    return 0;
}

static int txf_src_ip_inc_v6(int port, int pcore)
{
    int begin_ip = get_end_l2(port);
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].src_ip_inc))
    {
        int64_t p = *(uint64_t*)(data + begin_ip + 8 + 8);
        p += port_setup[port].src_ip_inc;
        if (p < port_setup[port].src_ip_min)
            p = port_setup[port].src_ip_max;
        else if (p > port_setup[port].src_ip_max)
            p = port_setup[port].src_ip_min;
        *(uint64_t*)(data+begin_ip + 8 + 8) = p;
    }
    return 0;
}

static int txf_dest_ip_inc_v6(int port, int pcore)
{
    int begin_ip = get_end_l2(port);
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].dest_ip_inc))
    {
        int64_t p = *(uint64_t*)(data + begin_ip + 8 + 24);
        p += port_setup[port].dest_ip_inc;
        if (p < port_setup[port].dest_ip_min)
            p = port_setup[port].dest_ip_max;
        else if (p > port_setup[port].dest_ip_max)
            p = port_setup[port].dest_ip_min;
        *(uint64_t*)(data+begin_ip + 8 + 24) = p;
    }
    return 0;
}

int txf_dest_mac_inc(int port, int pcore)
{
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].dest_mac_inc))
    {
        uint64_t m;
        char* mac = data + get_end_pre_l2(port);

        m = mac_to_uint64(mac);
        m += port_setup[port].dest_mac_inc;
        if ((m < port_setup[port].dest_mac_min) || (m > port_setup[port].dest_mac_max))
            m = port_setup[port].dest_mac_min;
        uint64_to_mac(m, mac);
    }
    return 0;
}

int txf_src_mac_inc(int port, int pcore)
{
    char *data;
    port_setup_t *portx;

    portx = port_setup + port;
    data = portx->output_data[pcore];

    if (cvmx_unlikely(port_setup[port].src_mac_inc))
    {
        uint64_t m;
        char* mac = data + get_end_pre_l2(port) + MAC_ADDR_LEN;

        m = mac_to_uint64(mac);
        m += port_setup[port].src_mac_inc;
        if ((m < port_setup[port].src_mac_min) || (m > port_setup[port].src_mac_max))
            m = port_setup[port].src_mac_min;
        uint64_to_mac(m, mac);
    }
    return 0;
}

tx_task_t tgpio_get_txf_ip(int port, int is_src, int is_ipaddr)
{
    switch(port_setup[port].output_packet_type)
    {
    case PACKET_TYPE_HELP:
    case PACKET_TYPE_IPV4_UDP:
    case PACKET_TYPE_IPV4_TCP:
        if (is_src)
            if (is_ipaddr)
                return txf_src_ip_inc;
        else
            return txf_src_port_inc;
        else
            if (is_ipaddr)
                return txf_dest_ip_inc;
        else
            return txf_dest_port_inc;
        break;

    case PACKET_TYPE_IPV6_UDP:
    case PACKET_TYPE_IPV6_TCP:
        if (is_src)
            if (is_ipaddr)
                return txf_src_ip_inc_v6;
        else
            return txf_src_port_inc_v6;
        else
            if (is_ipaddr)
                return txf_dest_ip_inc_v6;
        else
            return txf_dest_port_inc_v6;
        break;

    case PACKET_TYPE_802_3_PAUSE:
    case PACKET_TYPE_CBFC_PAUSE:
    case PACKET_TYPE_CJPAT:
    default:
            break;
    }
    return NULL;
}

/**
 * Called every packet to increment parts of the packet as necessary.
 *
 * @param port
 * @param data
 */
static inline void tgpio_perform_tx_tasks(int port, int pcore)
{
    int i;

    for (i = 0; i < port_setup[port].tx_ntasks; i++)
        if (port_setup[port].tx_tasks[i])
            port_setup[port].tx_tasks[i](port, pcore);
}

static inline
uint64_t tgpio_get_cycle(void)
{
    return cvmx_get_cycle() << CYCLE_SHIFT;
}

/*
 * In case, when more than one core per port is used, the configurable
 * parameter portx->output_cycle_gap should divided among those cores
 * working in parallel. Thus:
 * 1. The portx->output_cycle_gap is increased by the number of cores:
 *        cycle_gap = portx->output_cycle_gap * portx->num_cores
 * 2. Each core has shifted packet sent time relative to another
 * core from the group.
 *        shift = portx->output_cycle_gap * (portx->num_cores - pcore)
 */
int tgpio_packet_transmitter(txpkt_params_t *txparam,
    cvmx_pko_command_word0_t *pko_cmd, cvmx_buf_ptr_t *hw_buffer)
{
    port_setup_t *portx;
    uint64_t cycle_gap;

    portx = port_setup + txparam->port;
    cycle_gap = portx->output_cycle_gap * portx->num_cores;

    if (cvmx_likely(portx->port_valid && *txparam->output_enable && cycle_gap &&
            txparam->pcount > 0))
    {
        if (cvmx_likely(tgpio_get_cycle() >= txparam->output_cycle))
        {
            cvmx_pko_send_packet_prepare(txparam->port, txparam->queue, CVMX_PKO_LOCK_NONE);
            tgpio_perform_tx_tasks(txparam->port, txparam->pcore);
            txparam->output_cycle += cycle_gap;
            pko_cmd->s.total_bytes = portx->output_packet_size + get_size_pre_l2(txparam->port);
            if (portx->do_checksum)
                pko_cmd->s.ipoffp1 = get_end_l2(txparam->port) + 1;
            else
                pko_cmd->s.ipoffp1 = 0;
            cvmx_pko_send_packet_finish(txparam->port, txparam->queue,
                *pko_cmd, *hw_buffer, CVMX_PKO_LOCK_NONE);
            txparam->pcount -= 1;

            /* If we aren't keeping up, start sending 4 more packets per iteration */
            if (cvmx_unlikely(tgpio_get_cycle() > txparam->output_cycle + 500 &&
                    (txparam->pcount > 4)))
            {
                uint64_t words[8] = {
                    pko_cmd->u64, hw_buffer->u64, pko_cmd->u64, hw_buffer->u64,
                    pko_cmd->u64, hw_buffer->u64, pko_cmd->u64, hw_buffer->u64
                };
                txparam->output_cycle += cycle_gap * 4;
                txparam->pcount -= 4;
                if (cvmx_likely(cvmx_cmd_queue_write(CVMX_CMD_QUEUE_PKO(txparam->queue),
                        0, 8, words) == CVMX_CMD_QUEUE_SUCCESS))
                    cvmx_pko_doorbell(txparam->port, txparam->queue, 8);
            }
            if (cvmx_unlikely(txparam->pcount == 0))
            {
                txparam->pcount = portx->output_count;
                *txparam->output_enable = 0;
                CVMX_SYNCW;
            }
            return 1;
        }
    }
    else
    {
        txparam->output_cycle = tgpio_get_cycle() + cycle_gap -
            portx->output_cycle_gap * txparam->pcore;
        txparam->pcount = portx->output_count;
    }
    return 0;
}

int tgpio_packet_transmitter_o3(txpkt_params_t *txparam)
{
#ifdef _PKO3_FULL_API
    cvmx_pko3_pdesc_t desc;
#else
    cvmx_buf_ptr_pki_t pki_ptr = {0};
#endif
    port_setup_t *portx;
    uint64_t cycle_gap;
    int res, i, pktlen;

    portx = port_setup + txparam->port;
    cycle_gap = portx->output_cycle_gap * portx->num_cores;
    pktlen = portx->output_packet_size + get_size_pre_l2(txparam->port);
#ifdef _PKO3_FULL_API
    cvmx_pko3_pdesc_init(&desc);
    cvmx_pko3_pdesc_buf_append(&desc, txparam->pkt_vaddr, pktlen, (unsigned)0);
    cvmx_pko3_pdesc_set_free(&desc, false);
#else
    pki_ptr.size = pktlen;
    pki_ptr.addr = txparam->pkt_paddr;
#endif
    if (cvmx_likely(portx->port_valid && *txparam->output_enable &&
            cycle_gap && txparam->pcount > 0))
    {
        if (cvmx_likely(tgpio_get_cycle() >= txparam->output_cycle))
        {
            tgpio_perform_tx_tasks(txparam->port, txparam->pcore);
            txparam->output_cycle += cycle_gap;
#ifdef _PKO3_FULL_API
            if (portx->do_checksum) {
                desc.hdr_s->s.l3ptr = get_end_l2(txparam->port);
                desc.hdr_s->s.ckl3 = 1;
            }
            res = cvmx_pko3_pdesc_transmit(&desc, txparam->queue, NULL);
#else
            res = cvmx_pko3_xmit_link_buf(txparam->queue, pki_ptr, pktlen, -1, NULL, NULL);
#endif
            txparam->pcount -= 1;
           /* If we aren't keeping up, start sending more packets per iteration. */
            if (cvmx_unlikely(tgpio_get_cycle() > (txparam->output_cycle + portx->burst_cycles)))
            {
                for(i = 0; i < portx->burst_pkts; i++)
                {
                    if (res != 0 || txparam->pcount == 0)
                        break;
#ifdef _PKO3_FULL_API
                    res = cvmx_pko3_pdesc_transmit(&desc, txparam->queue, NULL);
#else
                    res = cvmx_pko3_xmit_link_buf(txparam->queue, pki_ptr, pktlen, -1, NULL, NULL);
#endif
                    txparam->output_cycle += cycle_gap;
                    txparam->pcount -= 1;
                }
            }
            if (cvmx_unlikely(txparam->pcount == 0))
            {
                txparam->pcount = portx->output_count / portx->num_cores;
                if (txparam->pcore == 0)
                    txparam->pcount += portx->output_count % portx->num_cores;

                *txparam->output_enable = 0;
                CVMX_SYNCW;
            }
            return 1;
        }
    }
    else
    {
        txparam->output_cycle = tgpio_get_cycle() + cycle_gap -
            portx->output_cycle_gap * txparam->pcore;
        txparam->pcount = portx->output_count / portx->num_cores;
        if (txparam->pcore == 0)
            txparam->pcount += portx->output_count % portx->num_cores;
    }
    return 0;
}

