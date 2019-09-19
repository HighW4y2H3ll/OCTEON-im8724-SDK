#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
	int i;
	uint32_t val;
	uint32_t *rom;
	int size;
	FILE *file;
	int rc = 0;

	if (argc < 3) {
		printf("Usage: %s [input ROM file] [output C uint32_t array]\n",
		       argv[0]);
		return 5;
	}
	file = fopen(argv[1], "r");
	if (!file) {
		perror("open");
		return 1;
	}
	fseek(file, 0, SEEK_END);
	size = (int)ftell(file) / 4;
	rewind(file);

	printf("ROM image size is %d 10-bit words (%d 30-bit words)\n",
	       size, (size + 2) / 3);

	rom = (uint32_t *)calloc(size / 3, sizeof(uint32_t));

	/* Read in the array of 10-bit hex values and convert to 32-bits */
	for (i = 0; i < size - 2; i += 3) {
		rc = fscanf(file, "%x", &val);
		if (rc != 1) {
			printf("Error reading hex value from ROM input file %s\n",
			       argv[1]);
			rc = 3;
			goto fail;
		}
		if (val > 0x3ff) {
			printf("Error: value 0x%x out of range\n", val);
			rc = 4;
			goto fail;
		}
		rom[i / 3] = val;
		rc = fscanf(file, "%x", &val);
		if (rc != 1) {
			printf("Error reading hex value from ROM input file %s\n",
			       argv[1]);
			rc = 3;
			goto fail;
		}
		if (val > 0x3ff) {
			printf("Error: value 0x%x out of range\n", val);
			rc = 4;
			goto fail;
		}
		rom[i / 3] |= val << 10;
		rc = fscanf(file, "%x", &val);
		if (rc != 1) {
			printf("Error reading hex value from ROM input file %s\n",
			       argv[1]);
			rc = 3;
			goto fail;
		}
		if (val > 0x3ff) {
			printf("Error: value 0x%x out of range\n", val);
			rc = 4;
			goto fail;
		}
		rom[i / 3] |= val << 20;
		rom[i / 3] |= 0xc0000000;	/* 30 bits data */
	}
	printf("After div3, size: %d, i: %d\n", size, i);
	if (size - i == 2) {
		rc = fscanf(file, "%x", &val);
		if (rc != 1) {
			printf("Error reading hex value from ROM input file %s\n",
			       argv[1]);
			rc = 3;
			goto fail;
		}
		if (val > 0x3ff) {
			printf("Error: value 0x%x out of range\n", val);
			rc = 4;
			goto fail;
		}
		rom[i / 3] = val;
		rc = fscanf(file, "%x", &val);
		if (rc != 1) {
			printf("Error reading hex value from ROM input file %s\n",
			       argv[1]);
			rc = 3;
			goto fail;
		}
		if (val > 0x3ff) {
			printf("Error: value 0x%x out of range\n", val);
			rc = 4;
			goto fail;
		}
		rom[i / 3] |= val << 10;
		rom[i / 3] |= 0x80000000;	/* 20 bits data */
	} else if (size - i == 1) {
		rc = fscanf(file, "%x", &val);
		if (rc != 1) {
			printf("Error reading hex value from ROM input file %s\n",
			       argv[1]);
			rc = 3;
			goto fail;
		}
		if (val > 0x3ff) {
			printf("Error: value 0x%x out of range\n", val);
			rc = 4;
			goto fail;
		}
		printf("Adding last byte 0x%x\n", val | 0x40000000);
		rom[i / 3] = val;
		rom[i / 3] |= 0x40000000;	/* 10 bits data */
	}

	rc = 0;
	printf("Read %d values, ROM image size is 0x%x 30-bit words\n",
	       i, i / 3);
	fclose(file);
	file = fopen(argv[2], "w");
	for (i = 0; i < (size + 2) / 3; i++) {
		bool last = (i == ((size + 2) / 3) - 1);

		if (i % 6 == 0)
			fprintf(file, "\n\t0x%08x%s ", rom[i], last ? "" : ",");
		else
			fprintf(file, "0x%08x%s ", rom[i], last ? "" : ",");
	}
fail:
	fclose(file);
	return rc;
}
