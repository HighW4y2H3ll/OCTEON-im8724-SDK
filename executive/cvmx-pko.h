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
#ifndef __CVMX_PKO_H__
#define __CVMX_PKO_H__

#include "cvmx-hwpko.h"

#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * @INTERNAL
 *
 * Backward compatibility for packet transmission using legacy PKO command.
 *
 */
extern cvmx_pko_return_value_t cvmx_pko3_legacy_xmit(unsigned dq,
	cvmx_pko_command_word0_t pko_command,
	cvmx_buf_ptr_t packet, uint64_t addr, bool tag_sw);

/**
 * Complete packet output. cvmx_pko_send_packet_prepare() must be called exactly once before this,
 * and the same parameters must be passed to both cvmx_pko_send_packet_prepare() and
 * cvmx_pko_send_packet_finish().
 *
 * WARNING: This function may have to look up the proper PKO port in
 * the IPD port to PKO port map, and is thus slower than calling
 * cvmx_pko_send_packet_finish_pkoid() directly if the PKO port
 * identifier is known.
 *
 * @param ipd_port   The IPD port corresponding the to pko port the packet is for
 * @param queue  Queue to use
 * @param pko_command
 *               PKO HW command word
 * @param packet Packet to send
 * @param use_locking
 *               CVMX_PKO_LOCK_NONE, CVMX_PKO_LOCK_ATOMIC_TAG, or CVMX_PKO_LOCK_CMD_QUEUE
 *
 * @return returns CVMX_PKO_SUCCESS on success, or error code on failure of output
 */
static inline cvmx_pko_return_value_t cvmx_pko_send_packet_finish(uint64_t ipd_port, uint64_t queue,
							    cvmx_pko_command_word0_t pko_command, cvmx_buf_ptr_t packet, cvmx_pko_lock_t use_locking)
{
	cvmx_cmd_queue_result_t result;

	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		return cvmx_pko3_legacy_xmit( queue, pko_command, packet, 0,
			use_locking == CVMX_PKO_LOCK_ATOMIC_TAG);
	}

	if (use_locking == CVMX_PKO_LOCK_ATOMIC_TAG)
		cvmx_pow_tag_sw_wait();

	result = cvmx_cmd_queue_write2(CVMX_CMD_QUEUE_PKO(queue), (use_locking == CVMX_PKO_LOCK_CMD_QUEUE), pko_command.u64, packet.u64);
	if (cvmx_likely(result == CVMX_CMD_QUEUE_SUCCESS)) {
		cvmx_pko_doorbell(ipd_port, queue, 2);
		return CVMX_PKO_SUCCESS;
	} else if ((result == CVMX_CMD_QUEUE_NO_MEMORY) || (result == CVMX_CMD_QUEUE_FULL)) {
		return CVMX_PKO_NO_MEMORY;
	} else {
		return CVMX_PKO_INVALID_QUEUE;
	}
}

/**
 * Complete packet output. cvmx_pko_send_packet_prepare() must be called exactly once before this,
 * and the same parameters must be passed to both cvmx_pko_send_packet_prepare() and
 * cvmx_pko_send_packet_finish().
 *
 * WARNING: This function may have to look up the proper PKO port in
 * the IPD port to PKO port map, and is thus slower than calling
 * cvmx_pko_send_packet_finish3_pkoid() directly if the PKO port
 * identifier is known.
 *
 * @param ipd_port   The IPD port corresponding the to pko port the packet is for
 * @param queue  Queue to use
 * @param pko_command
 *               PKO HW command word
 * @param packet Packet to send
 * @param addr   Plysical address of a work queue entry or physical address to zero on complete.
 * @param use_locking
 *               CVMX_PKO_LOCK_NONE, CVMX_PKO_LOCK_ATOMIC_TAG, or CVMX_PKO_LOCK_CMD_QUEUE
 *
 * @return returns CVMX_PKO_SUCCESS on success, or error code on failure of output
 */
static inline cvmx_pko_return_value_t cvmx_pko_send_packet_finish3(uint64_t ipd_port, uint64_t queue,
							     cvmx_pko_command_word0_t pko_command, cvmx_buf_ptr_t packet, uint64_t addr, cvmx_pko_lock_t use_locking)
{
	cvmx_cmd_queue_result_t result;

	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		return cvmx_pko3_legacy_xmit( queue, pko_command, packet, addr,
			use_locking == CVMX_PKO_LOCK_ATOMIC_TAG);
	}

	if (use_locking == CVMX_PKO_LOCK_ATOMIC_TAG)
		cvmx_pow_tag_sw_wait();

	result = cvmx_cmd_queue_write3(CVMX_CMD_QUEUE_PKO(queue), (use_locking == CVMX_PKO_LOCK_CMD_QUEUE), pko_command.u64, packet.u64, addr);
	if (cvmx_likely(result == CVMX_CMD_QUEUE_SUCCESS)) {
		cvmx_pko_doorbell(ipd_port, queue, 3);
		return CVMX_PKO_SUCCESS;
	} else if ((result == CVMX_CMD_QUEUE_NO_MEMORY) || (result == CVMX_CMD_QUEUE_FULL)) {
		return CVMX_PKO_NO_MEMORY;
	} else {
		return CVMX_PKO_INVALID_QUEUE;
	}
}
/**
 * Complete packet output. cvmx_pko_send_packet_prepare() must be called exactly once before this,
 * and the same parameters must be passed to both cvmx_pko_send_packet_prepare() and
 * cvmx_pko_send_packet_finish_pkoid().
 *
 * @param pko_port   Port to send it on
 * @param queue  Queue to use
 * @param pko_command
 *               PKO HW command word
 * @param packet Packet to send
 * @param use_locking
 *               CVMX_PKO_LOCK_NONE, CVMX_PKO_LOCK_ATOMIC_TAG, or CVMX_PKO_LOCK_CMD_QUEUE
 *
 * @return returns CVMX_PKO_SUCCESS on success, or error code on failure of output
 */
static inline cvmx_pko_return_value_t cvmx_pko_send_packet_finish_pkoid(int pko_port, uint64_t queue,
								  cvmx_pko_command_word0_t pko_command, cvmx_buf_ptr_t packet, cvmx_pko_lock_t use_locking)
{
	cvmx_cmd_queue_result_t result;

	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		return cvmx_pko3_legacy_xmit( queue, pko_command, packet, 0,
			use_locking == CVMX_PKO_LOCK_ATOMIC_TAG);
	}

	if (use_locking == CVMX_PKO_LOCK_ATOMIC_TAG)
		cvmx_pow_tag_sw_wait();
	result = cvmx_cmd_queue_write2(CVMX_CMD_QUEUE_PKO(queue), (use_locking == CVMX_PKO_LOCK_CMD_QUEUE), pko_command.u64, packet.u64);
	if (cvmx_likely(result == CVMX_CMD_QUEUE_SUCCESS)) {
		cvmx_pko_doorbell_pkoid(pko_port, queue, 2);
		return CVMX_PKO_SUCCESS;
	} else if ((result == CVMX_CMD_QUEUE_NO_MEMORY) || (result == CVMX_CMD_QUEUE_FULL)) {
		return CVMX_PKO_NO_MEMORY;
	} else {
		return CVMX_PKO_INVALID_QUEUE;
	}
}

/**
 * Complete packet output. cvmx_pko_send_packet_prepare() must be called exactly once before this,
 * and the same parameters must be passed to both cvmx_pko_send_packet_prepare() and
 * cvmx_pko_send_packet_finish_pkoid().
 *
 * @param pko_port   The PKO port the packet is for
 * @param queue  Queue to use
 * @param pko_command
 *               PKO HW command word
 * @param packet Packet to send
 * @param addr   Plysical address of a work queue entry or physical address to zero on complete.
 * @param use_locking
 *               CVMX_PKO_LOCK_NONE, CVMX_PKO_LOCK_ATOMIC_TAG, or CVMX_PKO_LOCK_CMD_QUEUE
 *
 * @return returns CVMX_PKO_SUCCESS on success, or error code on failure of output
 */
static inline cvmx_pko_return_value_t cvmx_pko_send_packet_finish3_pkoid(uint64_t pko_port, uint64_t queue,
								   cvmx_pko_command_word0_t pko_command, cvmx_buf_ptr_t packet, uint64_t addr, cvmx_pko_lock_t use_locking)
{
	cvmx_cmd_queue_result_t result;

	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		return cvmx_pko3_legacy_xmit( queue, pko_command, packet, addr,
			use_locking == CVMX_PKO_LOCK_ATOMIC_TAG);
	}

	if (use_locking == CVMX_PKO_LOCK_ATOMIC_TAG)
		cvmx_pow_tag_sw_wait();
	result = cvmx_cmd_queue_write3(CVMX_CMD_QUEUE_PKO(queue), (use_locking == CVMX_PKO_LOCK_CMD_QUEUE), pko_command.u64, packet.u64, addr);
	if (cvmx_likely(result == CVMX_CMD_QUEUE_SUCCESS)) {
		cvmx_pko_doorbell_pkoid(pko_port, queue, 3);
		return CVMX_PKO_SUCCESS;
	} else if ((result == CVMX_CMD_QUEUE_NO_MEMORY) || (result == CVMX_CMD_QUEUE_FULL)) {
		return CVMX_PKO_NO_MEMORY;
	} else {
		return CVMX_PKO_INVALID_QUEUE;
	}
}

#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif /* __CVMX_PKO_H__ */
