/***********************license start***************
 * Copyright (c) 2003-2014  Cavium Inc. (support@cavium.com). All rights
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
 * New lockless timer API, inspired by ODP timer definition.
 *
 * <hr>$Revision: 137666 $<hr>
 */

#ifndef	__CVMX_TIM_ATOMIC_H
#define	__CVMX_TIM_ATOMIC_H

#include "cvmx-wqe.h"

#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/* Timer handle is opaque to application */
typedef union cvmx_timer_info_u *cvmx_timer_id_t;

typedef enum {
	CVMX_TIMER_STATUS_SUCCESS = 0,
	CVMX_TIMER_STATUS_NO_MEMORY = -1,
	CVMX_TIMER_STATUS_TOO_FAR_AWAY = -9,
	CVMX_TIMER_STATUS_MISMATCH = -2,
	CVMX_TIMER_STATUS_BUSY = -3,
	CVMX_TIMER_STATUS_HANDLE = -4,
	CVMX_TIMER_STATUS_NOT_FOUND = -5,
} cvmx_timer_status_t;

/**
 * This is an opaque structure that is used as a handle into
 * a specific timeout, so it can be cancelled prior to its expiration.
 */
typedef struct {
	uint64_t word[2];
} cvmx_timer_handle_t;

extern cvmx_timer_status_t
cvmx_timer_arm_absolute(cvmx_timer_id_t timer_id, uint64_t abs_tick,
		cvmx_wqe_t *wqe, cvmx_timer_handle_t *handle_ptr);

extern cvmx_timer_status_t
cvmx_timer_arm_relative(cvmx_timer_id_t timer_id, int64_t ticks_from_now,
		cvmx_wqe_t *wqe, cvmx_timer_handle_t *handle_ptr);

extern cvmx_wqe_t *
cvmx_timer_cancel(cvmx_timer_id_t timer_id, cvmx_timer_handle_t *handle_ptr);

extern uint64_t
cvmx_timer_current_tick(cvmx_timer_id_t timer_id);

extern uint64_t
cvmx_timer_maximum_tmo(cvmx_timer_id_t timer_id);

extern uint64_t
cvmx_timer_ns_to_tick(cvmx_timer_id_t timer_id, uint64_t ns);

extern uint64_t
cvmx_timer_tick_to_ns(cvmx_timer_id_t timer_id, uint64_t ticks);

extern cvmx_timer_id_t
cvmx_timer_create(const char *name, unsigned node, unsigned pool,
		uint64_t min_tmo, uint64_t max_tmo);

extern cvmx_timer_id_t
cvmx_timer_id_from_name(const char *name);

extern cvmx_timer_status_t
cvmx_timer_destroy(cvmx_timer_id_t timer_id, const char *name);

#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif	/* __CVMX_TIM_ATOMIC_H */
