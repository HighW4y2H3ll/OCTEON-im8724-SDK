/*
 * Copyright 2012 Cavium, Inc. <support@cavium.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#ifndef __PARTITION_H__
#define __PARTITION_H__

/**
 * Offset of partition table in the master boot record
 */
#define DOS_PARTITION_OFFSET	0x1BE

/**
 * DOS partition table format.
 * 
 * See http://en.wikipedia.org/wiki/Master_boot_record for more details.
 * 
 * The actual partition table is located in start.S
 */
struct partition {
	uint8_t		status;
	uint8_t		start_head;
	uint8_t		start_cyl_hi:2;
	uint8_t		start_sector:6;
	uint8_t		start_cyl_lo;
	uint8_t		type;
	uint8_t		end_cyl_hi:2;
	uint8_t		end_sector:6;
	uint8_t		end_cyl_lo;
	uint32_t	start_lba;
	uint32_t	num_sectors;
} __attribute__((aligned(2)));


#endif /* __PARTITION_H__ */