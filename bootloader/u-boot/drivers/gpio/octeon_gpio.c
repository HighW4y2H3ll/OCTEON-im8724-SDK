/***********************license start***************
 * Copyright (c) 2011  Cavium Inc. (support@cavium.com). All rights
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
 *
 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.
 *
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

#include <common.h>
#include <asm/gpio.h>
#include <errno.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-gpio.h>
#include <asm/arch/octeon-model.h>

DECLARE_GLOBAL_DATA_PTR;

/* node is coded in gpio[9:8], so gpio# are node#0:0..19, #1:256..275 and so on */
#define node_of(gpio)		((gpio >> 8) & 3)
#define gpio_of(gpio)		(gpio & 0xFF)
#define is_node_exists(gpio)	( (1 << node_of(gpio)) & gd->arch.node_mask )

static int gpio_in_range(unsigned gpio)
{
	/* first check if the OCI setup detected requested node */
	if ( !is_node_exists(gpio) )
		return 0;
	else if (OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		 OCTEON_IS_MODEL(OCTEON_CNF75XX))
		return gpio_of(gpio) < 32;
	else if (OCTEON_IS_MODEL(OCTEON_CN66XX) || OCTEON_IS_MODEL(OCTEON_CN61XX)
		 || OCTEON_IS_MODEL(OCTEON_CNF71XX) || OCTEON_IS_OCTEON3() )
		return gpio_of(gpio) < 20;
	else
		return gpio_of(gpio) < 16;
}

void gpio_toggle_value(unsigned gpio)
{
	uint8_t node;
	uint64_t mask;

	if (!gpio_in_range(gpio))
		return;
	node = node_of(gpio);
	mask = 1ull << gpio_of(gpio);
	if (cvmx_gpio_read_node(node) && mask)
		cvmx_gpio_clear_node(node, mask);
	else
		cvmx_gpio_set_node(node, mask);
}

int gpio_direction_input(unsigned gpio)
{
	if (!gpio_in_range(gpio))
		return -1;
	cvmx_gpio_cfg_node(node_of(gpio), gpio_of(gpio), 0);
	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	uint8_t node;
	uint64_t mask;

	if (!gpio_in_range(gpio))
		return -1;
	node = node_of(gpio);
	mask = 1ull << gpio_of(gpio);
	cvmx_gpio_cfg_node(node, gpio_of(gpio), 1);
	if (OCTEON_IS_OCTEON3())
		cvmx_gpio_cfg_sel(node, gpio_of(gpio), 0);
	if (value)
		cvmx_gpio_set_node(node, mask);
	else
		cvmx_gpio_clear_node(node, mask);
	return 0;
}

int gpio_get_value(unsigned gpio)
{
	if (!gpio_in_range(gpio))
		return -1;
	return (cvmx_gpio_read_node(node_of(gpio)) & (1ull << gpio_of(gpio))) ? 1 : 0;
}

int gpio_set_value(unsigned gpio, int value)
{
	if (!gpio_in_range(gpio))
		return -1;
	if (value)
		cvmx_gpio_set_node(node_of(gpio), 1ull << gpio_of(gpio));
	else
		cvmx_gpio_clear_node(node_of(gpio), 1ull << gpio_of(gpio));
	return 0;
}

int gpio_request(unsigned gpio, const char *label)
{
	if (!gpio_in_range(gpio))
		return -EINVAL;

	return 0;
}

int gpio_free(unsigned gpio)
{
	return 0;
}
