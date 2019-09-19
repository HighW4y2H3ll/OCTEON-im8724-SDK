/*
 * (C) Copyright 2012 Cavium, Inc. <support@cavium.com>
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

#ifndef __OCTEON_BOARD_VSC8488_H__
#define __OCTEON_BOARD_VSC8488_H__

/**
 * This file contains routines common for the Vitesse VSC8488 PHY device
 * based boards, especially the SNIC10xxx boards.
 */

/**
 * Configures all Vitesse VSC8488 devices found.
 *
 * @return 0 for success, error otherwise.
 */
int octeon_vsc8488_config(void);

/**
 * This function tunes the QLM for use by the Vitesse VSC8488 PHY.
 *
 * @param qlm - qlm number to tune
 */
void octeon_board_vsc8488_qlm_tune(int qlm);

#endif /* __OCTEON_BOARD_VSC8488_H__ */
