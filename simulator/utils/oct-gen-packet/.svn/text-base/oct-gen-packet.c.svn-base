
#include <stdio.h>
#include <stdint.h>

uint32_t table[256];

int usage(char *execName)
{
	fprintf(stderr, "Usage: %s paylen destmac srcmac.\n", execName);
	return -1;
}

int make_table()
{
	int i;

	for (i = 0; i < 256; i++) {
		int rem = i, j;  /* remainder from polynomial division */
		for (j = 0; j < 8; j++) {
			if (rem & 1) {
				rem >>= 1;
				rem ^= 0xedb88320;
			} else
				rem >>= 1;
		}
		table[i] = rem;
	}
	return 0;
}

uint32_t block_crc(uint8_t *buff, int len, uint32_t crc_start)
{
	uint32_t crc = crc_start;

	while(len--)
		crc = (crc >> 8)  ^ table[(crc & 0xff) ^ *(buff++)];

	return ~crc;
}
extern uint32_t crc32(uint32_t seed, const char *p, unsigned size);

int main(int argc, char **argv)
{
	uint8_t pbuff[65536], *p = pbuff;
	int plen, i, blen;
	uint32_t crc;
	int phex = 1;

	if (argc < 4)
		return usage(argv[0]);
	if ((sscanf(argv[1], "%i", &plen) < 1) || ((plen > 65535) || (plen < 10)))
		return usage(argv[0]);

	if (strcmp(argv[0], "gfh") == 0) phex = 0;

	make_table();

	for (i = 0; i < 6; i++)
		sscanf(argv[2] + (i << 1), "%2hhx", p++);
	for (i = 0; i < 6; i++)
		sscanf(argv[3] + (i << 1), "%2hhx", p++);

	while(plen) {
		blen = (plen > 16) ? 16 : plen;
		for (i = 0; i < blen; i++) {
			*(p++) = 'a' + i;
		}
		plen -= blen;
	}

	crc = crc32(~0, pbuff, p - pbuff);

	//fprintf(stderr, "%x.\n", crc);

	*(p++) = crc & 0xff;
	crc >>= 8;
	*(p++) = crc & 0xff;
	crc >>= 8;
	*(p++) = crc & 0xff;
	crc >>= 8;
	*(p++) = crc & 0xff;
	crc >>= 8;

	if (phex) {
		uint8_t *wp = pbuff;
		int i;

		printf("%s %i %i\n", argv[4], p - pbuff, 1);

		while (wp < p) {
			printf("%s 0x", argv[4]);
			for(i = 0; i < 8; i++)
			{
				if (wp < p)
					printf("%02hhx", *(wp++));
				else
					printf("00");
				
			}
			printf("\n");
		}
	} else
		write(1, pbuff, p - pbuff);

	return 0;
}
