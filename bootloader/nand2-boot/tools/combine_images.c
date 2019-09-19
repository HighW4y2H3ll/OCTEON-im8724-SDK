/*
 * Copyright 2013 Cavium, Inc. <support@cavium.com>
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

/**
 * This program combines multiple binary images together into a single image
 * with programmible padding and block sizes.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "../spi-config.h"

void usage(const char *name)
{
	fprintf(stderr,
		"Usage: %s -p [pad] -i [increment value] -o [output filename] input file...\n"
		"Where:\n"
		"  -p pad	pad is the byte value to pad between input files in hex\n"
		"  -i value	value is the boundary size between images, padding up to\n"
		"               this value will be inserted between files.\n"
		"  -o filename  output filename, use - for stdout.\n"
		"  -h		Display this message\n"
		"  input file   multiple input filenames to combine into a single output image\n",
		name);
}

int main(int argc, char *argv[])
{
	FILE *ofile = stdout;
	FILE *ifile = NULL;
	uint8_t buffer[4096];
	int i;
	int c;
	const char *filename;
	long ilength;
	long size_left;
	size_t bytes_read;
	size_t bytes_written;
	size_t write_size;
	int header_inc = CONFIG_HEADER_SEARCH_INCREMENT;
	unsigned char pad_value = 0xFF;

	while ((c = getopt(argc, argv, "p:i:o:h")) != -1) {
		switch (c) {
		case 'p':
			pad_value = strtol(optarg, NULL, 16);
			break;
		case 'i':
			header_inc = strtol(optarg, NULL, 0);
			break;
		case 'o':
			if (!strcmp(optarg, "-"))
				ofile = stdout;
			else
				ofile = fopen(optarg, "w");
			if (!ofile) {
				fprintf(stderr, "Cannot open %s for writing\n",
					optarg);
				return -1;
			}
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		case '?':
			fprintf(stderr, "%s: Unrecognized option: -%c\n",
				argv[0], optopt);
			usage(argv[0]);
			return 1;
		default:
			fprintf(stderr, "%s: Unimplemented option: -%c\n",
				argv[0], c);
			usage(argv[0]);
			return 1;
		}
	}

	for (; optind < argc; optind++) {
		if (!access(argv[optind], R_OK)) {
			ifile = fopen(argv[optind], "r");
			if (!ifile) {
				fprintf(stderr, "Cannot open %s for reading\n",
					argv[optind]);
				if (ofile != stdout)
					fclose(ofile);
				return 1;
			}
			fseek(ifile, 0, SEEK_END);
			ilength = ftell(ifile);
			fseek(ifile, 0, SEEK_SET);
			size_left = ilength;
			do {
				bytes_read = fread(buffer, 1, sizeof(buffer), ifile);
				if (bytes_read < 0) {
					fprintf(stderr, "Error reading from %s\n",
						argv[optind]);
					return 2;
				}
				if (bytes_read &&
				    fwrite(buffer, 1, bytes_read, ofile) < bytes_read) {
					fprintf(stderr, "Error writing output\n");
					perror("fwrite");
					return 3;
				}
				size_left -= bytes_read;
			} while (size_left);
			fclose(ifile);
			if (optind < argc - 1) {
				memset(buffer, pad_value, sizeof(buffer));
				ilength = header_inc - (ilength % header_inc);
				/* Fix case where ilength == header_inc */
				ilength %= header_inc;
				printf("Adding %d pad bytes with value 0x%x\n",
				       ilength, pad_value);
				while (ilength) {
					write_size = (sizeof(buffer) < ilength) ?
						sizeof(buffer) : ilength;
					bytes_written = fwrite(buffer, 1,
							       write_size,
							       ofile);
					ilength -= bytes_written;
				};
			}
		}
	}
	if (ofile != stdout)
		fclose(ofile);
}
