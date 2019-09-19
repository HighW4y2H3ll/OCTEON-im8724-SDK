#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <zlib.h>
#include <arpa/inet.h>
#include <cvmx-bootloader.h>

struct env_format {
	uint32_t crc;
	char buffer[U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE];
};

int main(int argc, char *argv[])
{
	struct env_format env;
	FILE *infile, *outfile;
	char *s, *last;
	size_t size;
	uint32_t crc;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s [input file] [output file]\n", argv[0]);
		return 1;
	}

	infile = fopen(argv[1], "r");
	if (!infile) {
		perror("input file open");
		return 1;
	}
	outfile = fopen(argv[2], "w");
	if (!outfile) {
		perror("output file open");
		return 1;
	}
	memset(&env, 0, sizeof(env));

	if (fread(env.buffer, 1, sizeof(env.buffer)-2, infile) <= 0) {
		fprintf(stderr, "Error reading input environment file %s\n",
			argv[1]);
		return 1;
	}

	s = &env.buffer[0];
	last = s;
	do {
		strsep(&s, "\n\r");
		if (s)
			last = s;
	} while (s && *s);

	size = last - &env.buffer[0] + strlen(last) + 2;
	crc = crc32(0, &env.buffer[0], sizeof(env.buffer));
	env.crc = htonl(crc);

	if (fwrite(&env, sizeof(env), 1, outfile) != 1) {
		fprintf(stderr, "Error writing %s\n", argv[2]);
		return 1;
	}
	fclose(infile);
	fclose(outfile);
	return 0;
}