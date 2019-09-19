/*
 * (C) Copyright 2011
 * Cavium Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/**
 * @file octeon_hal.h
 *
 * $Id: octeon_hal.h 126272 2015-10-06 19:22:04Z cchavva $
 *
 */

#ifndef __OCTEON_HAL_H__
#define __OCTEON_HAL_H__

#if !defined(__U_BOOT__)
#include <cvmx.h>
#else
#include "octeon_csr.h"
#endif

/* Provide alias for __octeon_is_model_runtime__ */
#define octeon_is_model(x)     __octeon_is_model_runtime__(x)

#define OCTEON_PCI_IOSPACE_BASE     0x80011a0400000000ull

static inline int cvmx_octeon_fuse_locked (void)
{
	return cvmx_fuse_read (123);
}

#endif
