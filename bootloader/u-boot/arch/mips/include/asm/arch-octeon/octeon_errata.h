/*
 * (C) Copyright 2013 Cavium, Inc. <support@cavium.com>
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
 *
 * This file deals with working around various errata.
 */
#ifndef __OCTEON_ERRATA_H__
#define __OCTEON_ERRATA_H__

/**
 * This function works around various chip errata early in the initialization
 * process.
 *
 * @return 0 for success
 */
int octeon_early_errata_workaround_f(void);

/**
 * This function works around various chip errata late in the initialization
 * process.
 *
 * @return 0 for success
 */
int octeon_late_errata_workaround_f(void);

#endif /* __OCTEON_ERRATA_H__ */
