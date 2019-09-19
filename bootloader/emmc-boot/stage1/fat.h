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
#ifndef __FAT_H__
#define __FAT_H__

#define FAT_SECTOR_SIZE		512

#define FAT_ENT_FREE	0
#define FAT_EOF_FAT12	0xFFF
#define FAT_EOF_FAT16	0xFFFF
#define FAT_EOF_FAT32	0x0FFFFFFF
#define FAT_BOOT_SIGNATURE	0x55AA

typedef enum {
	floppy_1_44MB = 0xf0,
	floppy_1_2MB = 0xf9,
	floppy_720KB = 0xf9,
	floppy_360KB = 0xfd,
	floppy_320KB = 0xff,
	floppy_180KB = 0xfc,
	floppy_160KB = 0xfe,
	hard_drive = 0xf8
} fat_media_descriptor_t;

struct fat_table_sector {
	union {
		uint16_t	fat16[FAT_SECTOR_SIZE/sizeof(uint16_t)];
		uint32_t	fat32[FAT_SECTOR_SIZE/sizeof(uint32_t)];
	};
};

/**
 * FAT boot sector
 */
struct fat_boot_sector {
	union {
		struct {
			uint8_t		jmp[3];		/* 0x00 */
			uint8_t		system_id[8];	/* 0x03 */
			uint16_t	sector_size;	/* 0x0b bytes per logical sector*/
			uint8_t		sec_per_clus;	/* 0x0d sectors/cluster */
			uint16_t	reserved;	/* 0x0e reserved sectors */
			uint8_t		fats;		/* 0x10 number of fats */
			uint16_t	dir_entries;	/* 0x11 root directory entries */
			uint16_t	sectors;	/* 0x13 number of sectors */
			uint8_t		media;		/* 0x15 media code */
			uint16_t	fat_length;	/* 0x16 sectors/FAT */
			uint16_t	secs_track;	/* 0x18 sectors/track */
			uint16_t	heads;		/* 0x1a number of heads */
			uint32_t	hidden_sectors;	/* 0x1c num hidden sectors */
			uint32_t	total_sect;	/* number of sectors (if sectors==0) */

			/* Used by FAT32 only */
			uint32_t	fat32_length;	/* 0x24 - sectors/FAT */
			uint16_t	flags;		/* 0x28 bit 8: fat mirring, low 4: active fat */
			uint16_t	version;	/* 0x2a major, minor filesystem version */
			uint32_t	root_cluster;	/* 0x2c first cluster in root directory */
			uint16_t	info_sector;	/* 0x30 filesystem info sector */
			uint16_t	backup_boot;	/* 0x32 backup boot sector */
			uint8_t	reserved2[0x1ca]; /* 0x34 reserved */
			uint16_t	signature;	/* 0x55AA (big endian) */
		} __attribute__((packed));
		uint8_t buffer[FAT_SECTOR_SIZE];
	};
} __attribute__((packed));

/**
 * FAT32 Info Sector
 */
struct fat_info_sector {
	uint32_t	info_sector_signature;	/* 0x00 (0x52 0x52 0x61 0x41 = 'RRaA') */
	uint8_t		reserved[480];		/* 0x04 */
	uint32_t	info_sector_signature2;	/* 0x1e4 (0x72 0x72 0x41 0x61 = 'rrAa') */
	uint32_t	num_free_clusters;	/* 0x1e8 */
	uint32_t	last_allocated_cluster;	/* 0x1ec - most recently allocated data cluster */
	uint8_t		reserved2[12];
	uint32_t	info_sector_signature3;	/* 0x1fc - 0x00 0x00 0x55 0xaa */
} __attribute__((packed));

/* Attribute values in the directory entry */
#define FAT_ATTR_READ_ONLY		1
#define FAT_ATTR_HIDDEN			2
#define FAT_ATTR_SYSTEM			4
#define FAT_ATTR_VOLUME_LABEL		8
#define FAT_ATTR_DIRECTORY		0x10
#define FAT_ATTR_ARCHIVE		0x20
#define FAT_ATTR_DEVICE			0x40
/**
 * FAT directory entry
 */
struct fat_dir_entry {
	uint8_t		shortname[11];		/* 0x00 8.3 filename */
	uint8_t		attributes;		/* 0x0b file attributes */
	uint8_t		reserved;		/* 0x0c */
	uint8_t		create_time_fine;	/* 0x0d Creation time, fine resolution in 10ms */
	uint16_t	creation_time;		/* 0x0e hhhhhmmmmmsssss */
	uint16_t	creation_date;		/* 0x10 yyyyyyymmmmddddd */
	uint16_t	access_date;		/* 0x12 last access date */
	uint16_t	high_first_cluster;	/* 0x14 - high 16-bits of first cluster for FAT32 */
	uint16_t	last_modify_time;	/* 0x16 */
	uint16_t	last_modify_date;	/* 0x18 */
	uint16_t	start_first_cluster;	/* 0x1a */
	uint32_t	file_size;		/* 0x1c */
};

struct fat_dir_info {
	uint8_t		shortname[11];
	uint8_t		attributes;
	uint32_t	start_cluster;
	uint32_t	file_size;
};

typedef enum {
	FAT_TYPE_FAT12 = 12,
	FAT_TYPE_FAT16 = 16,
	FAT_TYPE_FAT32 = 32
} fat_type_t;

struct fat_info {
	struct fat_table_sector fat_cache;	/* Cache holding fat table sector */
	uint32_t	last_fat_sector;
	uint32_t	start_sector;
	uint32_t	fat_start_sector;
	uint32_t	fat_length;
	uint32_t	total_sector_count;
	uint32_t	root_cluster;
	uint32_t	root_dir_sectors;
	uint32_t	root_size;
	uint32_t	cluster_start_sector;	/* Sector address of cluster 0 */
	fat_type_t	fat_type;
	uint16_t	sector_size;
	uint8_t		current_fat_table;
	uint8_t		cluster_size;		/* Size of cluster in sectors */
	uint8_t		num_fats;
};

/* FAT ROOT directory starting cluster */
#define FAT_ROOT_DIR_CLUS_FAT16		0

/*int fat_mount(struct mmc *mmc, struct fat_info *finfo, uint32_t start_sector);*/

#endif /* __FAT_H__ */
