/***********************license start***************
 * Copyright (c) 2014  Cavium Inc. (support@cavium.com).  All rights
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
#ifndef __BOOTOCT_CMD_H__
#define __BOOTOCT_CMD_H__

#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-coremask.h>

enum octeon_boot_cmd_type {
	BOOTOCT,
	BOOTOCTLINUX,
	BOOTOCTELF
};

/** Structure to contain results of command line argument parsing */
struct octeon_boot_args {
	cvmx_coremask_t coremask;	/** Parsed coremask */
	int num_cores[CVMX_MAX_NODES];	/** number of cores */
	int num_skipped[CVMX_MAX_NODES];/** number of skipped cores */
	const char *app_name;		/** Application name */
	const char *named_block;	/** Named block to load Linux into */
	uint32_t stack_size;		/** stack size */
	uint32_t heap_size;		/** heap size */
	uint32_t boot_flags;		/** boot flags */
	int node_mask;			/** Node mask to use */
	bool forceboot;			/** force booting if core 0 not set */
	bool coremask_set;		/** set if coremask was set */
	bool num_cores_set;		/** Set if num_cores was set */
	bool num_skipped_set;		/** Set if num_skipped was set */
	/** Set if endbootargs parameter was passed. */
	bool endbootargs;
};

/**
 * Parse command line arguments
 *
 * @param argc			number of arguments
 * @param[in] argv		array of argument strings
 * @param cmd			command type
 * @param[out] boot_args	parsed values
 *
 * @return number of arguments parsed
 */
int octeon_parse_bootopts(int argc, char *const argv[],
			  enum octeon_boot_cmd_type cmd,
			  struct octeon_boot_args *boot_args);

#endif /* __BOOTOCT_CMD_H__ */
