#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <endian.h>
#include <sys/mount.h>
#include "../stage1/partition.h"
#include <cvmx-bootloader.h>
#include <zlib.h>	/* For CRC32 */
#include <arpa/inet.h>

#ifndef le32toh
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define le32toh(x) (x)
#else
#define le32toh(x) __bswap_32(x)
#endif
#endif

int check_dev(int fd)
{
	struct stat st;
	int rc;

	rc = fstat(fd, &st);

	return !(!rc && S_ISBLK(st.st_mode));
}

int get_sector_size(int fd)
{
	int rc, sector_size;

	rc = ioctl(fd, BLKSSZGET, &sector_size);
	if (rc < 0)
		return -1;
	else
		return sector_size;
}

/**
 * Calculates the CRC over the stage 2 bootloader header while skipping the
 * CRC32 value embedded in the header
 * 
 * @param header - bootloader header
 *
 * @return CRC32 value
 */
static uint32_t calculate_header_crc(const bootloader_header_t * header)
{
	uint32_t crc;
	uint32_t hcrc_p;

	hcrc_p = offsetof(bootloader_header_t, hcrc);

	crc = crc32(0, (void *)header, hcrc_p);
	hcrc_p += sizeof(header->hcrc);	/* step up pass header crc field */
	crc =
	    crc32(crc, (void *)(((uint64_t) header) + hcrc_p),
		  ntohs(header->hlen) - hcrc_p);

	return crc;
}

size_t get_bootloader_size(void *buffer)
{
	bootloader_header_t *header = (bootloader_header_t *)buffer;

	return ntohl(header->dlen) + ntohs(header->hlen);
}

int validate_bootloader(void *buffer, size_t size)
{
	bootloader_header_t *header = (bootloader_header_t *)buffer;
	uint8_t *data;
	uint32_t crc;

	if (ntohl(header->magic) != BOOTLOADER_HEADER_MAGIC)
		return 1;
	if (ntohs(header->hlen) > BOOTLOADER_HEADER_MAX_SIZE)
		return 1;
	if ((ntohs(header->maj_rev) == 1) && (ntohs(header->min_rev) == 0))
		return 1;	/* Relocation not supported */
	if (size < ntohs(header->hlen) + ntohl(header->dlen))
		return 1;
	if (calculate_header_crc(header) != ntohl(header->hcrc))
		return 1;

	data = (uint8_t *)buffer;
	data += ntohs(header->hlen);

	crc = crc32(0, data, ntohl(header->dlen));
	if (crc != ntohl(header->dcrc)) {
		fprintf(stderr, "Bootloader data CRC mismatch, expected 0x%x, got 0x%x\n",
			ntohl(header->dcrc), crc);
		return 1;
	}
	return 0;
}

size_t get_file_size(FILE *f)
{
	off_t loc;
	size_t size;

	loc = ftell(f);
	if (loc < 0)
		return loc;
	if (fseek(f, 0, SEEK_END) != 0)
		return -1;
	size = ftell(f);
	if (fseek(f, loc, SEEK_SET) != 0)
		return -1;
	return size;
}

void usage(const char *progname)
{
	printf("%s [device] <[stage 2 bootloader]>\n", progname);
	puts("Where device is the device name for the MMC/SD adapter (i.e. /dev/sdb)\n"
	     "and the optional stage 2 bootloader is the stage 2 bootloader filename\n");
}

int main(int argc, char *argv[])
{
	char	part_buffer[512];
	char	stage1_file[1024];
	long	stage1_size;
	long	stage2_size;
	char	*devname;
	int	dev_fd;
	uint8_t	*buffer;
	FILE	*bootloader_file;
	char	*octeon_root;
	struct	partition *dos_partition;
	size_t	size;
	uint32_t part_start;
	int	i;
	int	found;
	int	rc = 0;

	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	devname = argv[1];

	dev_fd = open(devname, O_RDWR);
	if (dev_fd < 0) {
		perror("open");
		return 1;
	}

	if (check_dev(dev_fd)) {
		fprintf(stderr, "Invalid device %s\n", devname);
		return 1;
	}

	if (get_sector_size(dev_fd) != 512) {
		fprintf(stderr, "Invalid sector size, only 512 byte sectors are supported.\n");
		return 1;
	}

	size = read(dev_fd, part_buffer, sizeof(part_buffer));
	if ((long)size <= 0) {
		perror("read");
	}
	if (size != sizeof(part_buffer)) {
		fprintf(stderr, "Cannot read partition table sector\n");
		return 2;
	}

	dos_partition = (struct partition *)&part_buffer[DOS_PARTITION_OFFSET];

	octeon_root = getenv("OCTEON_ROOT");
	if (octeon_root == NULL) {
		fprintf(stderr, "Please source env-setup to set the OCTEON_ROOT "
				"environment variable.\n");
		return 3;
	}

	snprintf(stage1_file, sizeof(stage1_file), "%s/target/bin/emm-boot.bin",
		 octeon_root);

	bootloader_file = fopen(stage1_file, "r");
	if (bootloader_file == NULL) {
		fprintf(stderr, "Could not open stage 1 bootloader %s\n",
			stage1_file);
		return 4;
	}

	stage1_size = get_file_size(bootloader_file);
	if ((long)stage1_size <= 0) {
		fprintf(stderr, "Error obtaining stage 1 bootloader size\n");
		return 4;
	}

	if (stage1_size > 8192) {
		fprintf(stderr, "Error: stage 1 bootloader %s is %ld bytes long.  "
			"It must not exceed 8192 bytes\n", stage1_file,
			stage1_size);
		return 5;
	}

	rewind(bootloader_file);

	buffer = (uint8_t *)calloc(8192, 1);
	if (buffer == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 6;
	}

	size = fread(buffer, 1, stage1_size, bootloader_file);
	if ((long)size != stage1_size) {
		fprintf(stderr, "Could not read stage 1 bootloader file %s\n",
			stage1_file);
		return 4;
	}
	fclose(bootloader_file);

	dos_partition = (struct partition *)&part_buffer[DOS_PARTITION_OFFSET];

	/* Copy the partition table */
	memcpy(&buffer[DOS_PARTITION_OFFSET],
	       dos_partition, sizeof(struct partition) * 4);

	found = -1;
	part_start = 0x800;

	for (i = 0; i < 4; i++) {
		if ((dos_partition[i].status == 0x80) &&
		    ((dos_partition[i].type == 0x4) ||	/* FAT16 < 32M */
		     (dos_partition[i].type == 0x6) ||	/* FAT16 */
		     (dos_partition[i].type == 0x14) ||	/* hidden FAT16 < 32M */
		     (dos_partition[i].type == 0x16) ||	/* hidden FAT16 */
		     (dos_partition[i].type == 0xB) ||	/* FAT32 */
		     (dos_partition[i].type == 0xC) ||	/* FAT32 LBA */
		     (dos_partition[i].type == 0xE) ||	/* FAT16 LBA */
		     (dos_partition[i].type == 0x1B) ||	/* hidden FAT32 */
		     (dos_partition[i].type == 0x1C) ||	/* hidden LBA FAT32 */
		     (dos_partition[i].type == 0x1E)))	/* hidden LBA FAT16 */
			found = i;
		if ((le32toh(dos_partition[i].start_lba) < part_start) &&
		    (le32toh(dos_partition[i].start_lba) > 0))
			part_start = le32toh(dos_partition[i].start_lba);
	}

	if (found == -1) 
		printf("Warning: No bootable FAT partition found.\n");

	if (part_start < 16) {
		fprintf(stderr, "The stage 1 bootloader requires that the first "
			"8192 bytes not be used.  Please repartition.\n");
		fprintf(stderr, "The first partition starts at %u\n", part_start);
		return 7;
	}

	rc = lseek(dev_fd, 0, SEEK_SET);
	if (rc != 0) {
		fprintf(stderr, "Could not seek to beginning of device %s\n",
			devname);
		return 1;
	}

	printf("Writing %ld bytes for stage 1\n", stage1_size);

	size = write(dev_fd, buffer, (stage1_size + 511) & ~511);
	if ((long)size < stage1_size) {
		fprintf(stderr, "Could not write new boot loader");
		return 8;
	}

	free(buffer);
	buffer = NULL;

	/* Done with stage 1 bootloader, what about a stage 2 bootloader? */
	if (argc > 2) {
		bootloader_file = fopen(argv[2], "r");
		if (bootloader_file == NULL) {
			fprintf(stderr, "Could not open stage 2 bootloader file %s\n",
				argv[2]);
			return 9;
		}

		stage2_size = get_file_size(bootloader_file);
		if (stage2_size < BOOTLOADER_HEADER_MAX_SIZE) {
			fprintf(stderr, "Error: Stage 2 bootloader too small\n");
			return 9;
		}

		for (i = 0; i < 4; i++) {
			if (dos_partition[i].num_sectors == 0)
				continue;
			if (stage2_size >= le32toh(dos_partition[i].start_lba) * 512) {
				fprintf(stderr, "Not enough room to write stage "
					"2 bootloader in front of partition %d\n"
					"Partition %d starts at sector %u but the "
					"stage 2 bootloader requires it start after "
					"sector %u.\n",
					i+1, i+1,
					(uint32_t)le32toh(dos_partition[i].start_lba),
					(uint32_t)((stage2_size + 511 + 8192) / 512));
				return 9;
			}
		}

		buffer = (uint8_t *)malloc(stage2_size);
		if (buffer == NULL) {
			fprintf(stderr, "Error: Out of memory\n");
			return 9;
		}
		if (fread(buffer, 1, stage2_size, bootloader_file) != stage2_size) {
			fprintf(stderr, "Error reading stage 2 bootloader\n");
			return 9;
		}
		if (validate_bootloader(buffer, stage2_size)) {
			fprintf(stderr, "Error validating stage 2 bootloader\n");
			return 9;
		}
		rc = lseek(dev_fd, 8192, SEEK_SET);
		if (rc != 8192) {
			perror("lseek");
			return 9;
		}
		printf("Writing %ld bytes for stage 2\n", stage2_size);
		size = write(dev_fd, buffer, stage2_size);
		if ((long)size < stage2_size) {
			fprintf(stderr, "Could not write stage 2 bootloader\n");
			return 9;
		}
		fclose (bootloader_file);
		free(buffer);
	}

	close (dev_fd);

	return 0;
}

