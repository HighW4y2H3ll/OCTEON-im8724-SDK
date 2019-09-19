/***********************license start************************************
 * Copyright (c) 2011-2013 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * support@cavium.com
 *
 ***********************license end**************************************/

/**
 *
 * $Id: octeon_wd.c 163638 2017-07-26 02:24:50Z cchavva $
 *
 * Watchdog support imported from the Linux kernel driver
 */

#include <common.h>
#include <watchdog.h>
#include <asm/mipsregs.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access.h>
#include <asm/arch/cvmx-ciu-defs.h>
#include <asm/arch/cvmx-mio-defs.h>
#include <asm/arch/octeon_boot.h>
#ifndef CONFIG_SYS_NO_FLASH
#include <mtd/cfi_flash.h>
#endif
#include <asm/arch/octeon_boot_bus.h>
#include <asm/arch/cvmx-coremask.h>

DECLARE_GLOBAL_DATA_PTR;

#define K0		26
#define C0_CVMMEMCTL	11, 7
#define C0_STATUS	12, 0
#define C0_EBASE	15, 1
#define C0_DESAVE	31, 0

extern u64 octeon_get_io_clock_rate(void);

/* Defined in start.S */
extern void nmi_exception_handler(void);
extern void asm_reset(void);

static uint32_t octeon_watchdog_get_period(unsigned int msecs)
{
	uint64_t counter;
	uint64_t timeout_cnt;
	uint64_t clock_rate = octeon_get_io_clock_rate();

	msecs /= 2;

	/* Note: although I have been told that the counter for CN78XX should
	 * be 768 instead of 1024 that value does not seem to give accurate
	 * results.
	 */
	if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		counter = 0x1 << 9;
	else if (octeon_has_feature(OCTEON_FEATURE_CIU3))
		counter = 0x1 << 10;
	else
		counter = 0x1 << 8;

	if (msecs > 6000)
		msecs = 6000;

	/* NOTE: We divide the final result by 128 instead of 256 because the
	 * first interrupt is ignored, effectively doubling the time.
	 */
	do {
		timeout_cnt = (((clock_rate / counter) * msecs) / 1000) >> 8;
		debug("%s: msecs: %u, timeout count: %llu\n",
		      __func__, msecs, timeout_cnt);
		msecs -= 500;
	} while (timeout_cnt > 65535);
	debug("%s: msecs: %u, clock rate: %llu, counter: %llu\n",
	      __func__, msecs, clock_rate, counter);
	debug("%s: Setting timeout count to %llu\n", __func__, timeout_cnt);
	return (uint32_t)timeout_cnt;
}

/**
 * Starts the watchdog with the specified interval.
 *
 * @param msecs - time until NMI is hit.  If zero then use watchdog_timeout
 *                environment variable or default value.
 *
 * NOTE: if the time exceeds the maximum supported by the hardware then it
 * will be limited to the maximum supported by the hardware.
 */
void hw_watchdog_start(int msecs)
{
	union cvmx_ciu_wdogx ciu_wdog;
	static int initialized = 0;

	debug("%s(%d)\n", __func__, msecs);

	if (!initialized) {
		/* Initialize the stage1 code */
		boot_init_vector_t *p = octeon_get_boot_vector();
		if (!p) {
			/* Something bad happened */
			return;
		}
		p[0].code_addr = (uint64_t)(int64_t)(long)&nmi_exception_handler;
		initialized = 1;
	}

	if (!msecs)
		msecs = getenv_ulong("watchdog_timeout", 10,
				     CONFIG_OCTEON_WD_TIMEOUT * 1000);

	cvmx_write_csr(CVMX_CIU_PP_POKEX(0), 1);

	ciu_wdog.u64 = 0;
	ciu_wdog.s.len = octeon_watchdog_get_period(msecs);
	ciu_wdog.s.mode = 3;	/* Interrupt + NMI + soft-reset */

	cvmx_write_csr(CVMX_CIU_WDOGX(0), ciu_wdog.u64);
	cvmx_write_csr(CVMX_CIU_PP_POKEX(0), 1);
}

/**
 * Puts the dog to sleep
 */
void hw_watchdog_disable(void)
{
	union cvmx_ciu_wdogx ciu_wdog;
	/* Disable the hardware. */
	ciu_wdog.u64 = 0;

	/* Poke the watchdog to clear out its state */
	cvmx_write_csr(CVMX_CIU_PP_POKEX(0), 1);
	cvmx_write_csr(CVMX_CIU_WDOGX(0), ciu_wdog.u64);

	debug("%s: Watchdog stopped\n", __func__);
}

extern cvmx_coremask_t *octeon_get_available_coremask(cvmx_coremask_t *pcm);
extern uint64_t get_logical_coremask(uint64_t ciu_fuse_value);

/**
 * Disables all watchdogs on all cores and nodes
 */
void octeon_watchdog_disable_all(void)
{
	struct cvmx_coremask cm;
	union cvmx_ciu_wdogx ciu_wdog;
	int node;
	int core;

	octeon_get_available_coremask(&cm);

	ciu_wdog.u64 = 0;

	cvmx_coremask_for_each_core(core, &cm) {
		int local_core = cvmx_coremask_core_on_node(core);
		node = cvmx_coremask_core_to_node(core);
		cvmx_write_csr_node(node,
				    CVMX_CIU_PP_POKEX(local_core),
				    1);
		cvmx_write_csr_node(node,
				    CVMX_CIU_WDOGX(local_core),
				    ciu_wdog.u64);
	}
}

/**
 * Pets the watchdog
 */
void hw_watchdog_reset(void)
{
	/* Pet the dog.  Good dog! */
	cvmx_write_csr(CVMX_CIU_PP_POKEX(0), 1);
}

